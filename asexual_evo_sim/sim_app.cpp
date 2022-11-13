#include "pch.h"

#include "sim_app.h"

#include "creature_render_system.h"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <iostream>

#include <phm_buffer.h>

namespace evo
{
struct GlobalUbo
{
	glm::mat4 projection{ 1.0f };	
};

Application::Application()
	: imgui_context_(ImGui::CreateContext()), io_(ImGui::GetIO())
{
	globalPool_ = phm::DescriptorPool::Builder(device_)
		.setMaxSets(phm::Swapchain::MAX_FRAMES_IN_FLIGHT + 1)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, phm::Swapchain::MAX_FRAMES_IN_FLIGHT)
		.build();

	// Initialize ImGui
	ImGui::SetCurrentContext(imgui_context_);

	io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io_.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io_.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io_.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForVulkan(window_.getGLFWWindow(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	device_.populateImGui_initInfo(init_info);
	renderer_.populateImGui_initInfo(init_info);
	globalPool_->populateImGui_initInfo(init_info);
	ImGui_ImplVulkan_Init(&init_info, renderer_.getSwapChainRenderPass());

	// Upload Fonts
	{
		// Use any command queue
		VkCommandPool command_pool = device_.getCommandPool();
		VkCommandBuffer command_buffer = device_.beginSingleTimeCommands();

		///*if (!vkResetCommandPool(device_.device(), command_pool, 0))
		//{
		//	throw std::runtime_error("Failed to reset command pool");
		//}*/
		//VkCommandBufferBeginInfo begin_info = {};
		//begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		//if (!vkBeginCommandBuffer(command_buffer, &begin_info))
		//{
		//	throw std::runtime_error("Failed to begin recording command buffer for initializing fonts");
		//}

		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

		device_.endSingleTimeCommands(command_buffer);

		ImGui_ImplVulkan_DestroyFontUploadObjects();

		//globalPool_->resetPool();
	}

	//loadObjects();
}

Application::~Application()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(imgui_context_);
}

void Application::render_ui()
{

	ImGui::Begin("Info");

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Frame Time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
	//ImGui::SliderFloat2("Smooth Vase X and Y", &objects_[0].transform.translation.x, -5.0f, 5.0f);

	ImGui::End();



}

void Application::run()
{
	std::vector<std::unique_ptr<phm::Buffer>> uniformBuffers(phm::Swapchain::MAX_FRAMES_IN_FLIGHT);
	for (auto& bufferPtr : uniformBuffers)
	{
		bufferPtr = std::make_unique<phm::Buffer>(device_,
											 sizeof(GlobalUbo),
												  phm::Swapchain::MAX_FRAMES_IN_FLIGHT,
											 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
											 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
											 device_.properties.limits.minUniformBufferOffsetAlignment);
		bufferPtr->map();
	}

	auto globalSetLayout = phm::DescriptorSetLayout::Builder(device_)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.build();

	std::vector<VkDescriptorSet> globalDescriptorSets(phm::Swapchain::MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < globalDescriptorSets.size(); i++)
	{
		auto bufferInfo = uniformBuffers[i]->descriptorInfo();
		phm::DescriptorWriter(*globalSetLayout, *globalPool_)
			.writeBuffer(0, &bufferInfo)
			.build(globalDescriptorSets[i]);
	}

	CreatureRenderSystem simpleRenderSystem{ device_, renderer_.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

	while (!window_.shouldClose())
	{
		glfwPollEvents();

		float aspect = renderer_.getAspectRatio();

		// BeginFrame returns a nullptr if the swapchain needs to be recreated. 
		// This skips the frame draw call, if that's the case.
		auto commandBuffer = renderer_.beginFrame();
		if (commandBuffer != nullptr)
		{
			const FrameInfo frameInfo{
					renderer_.getFrameIndex(),
					commandBuffer,
					globalDescriptorSets[renderer_.getFrameIndex()]
			};

			// Update
			simulation_.creatures_[0].position.x += 0.00002;

			GlobalUbo ubo{};
			ubo.projection =
			{
				{ 1.0f / aspect, 0.0f, 0.0f, 0.0f },
				{ 0.0f, -1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			uniformBuffers[renderer_.getFrameIndex()]->writeToBuffer(&ubo);
			uniformBuffers[renderer_.getFrameIndex()]->flush();

			renderer_.beginSwapChainRenderPass(commandBuffer);
			// Render scene
			simpleRenderSystem.renderObjects(frameInfo, simulation_.creatures_);

			// Render GUI
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			render_ui();

			// End render
			ImGui::Render();
			auto drawData = ImGui::GetDrawData();
			ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
			// Update and Render additional Platform Windows
			if (io_.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			renderer_.endSwapChainRenderPass(commandBuffer);
			renderer_.endFrame();
		}
	}

	vkDeviceWaitIdle(device_.device());
}

}


