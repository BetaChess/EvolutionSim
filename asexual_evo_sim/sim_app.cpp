#include "pch.h"

#include "sim_app.h"

#include "simple_render_system.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <iostream>

std::vector<phm::Model::Vertex> sierpinskiTriangle(std::array<phm::Model::Vertex, 3> initialTriangle, size_t depth)
{
	if (depth == 0)
		return { initialTriangle[0], initialTriangle[1], initialTriangle[2] };

	auto triangles1 = sierpinskiTriangle(
		{
			initialTriangle[0],
			{
				{ // position
					initialTriangle[0].pos.x + (initialTriangle[1].pos.x - initialTriangle[0].pos.x) * 0.5f,
					initialTriangle[0].pos.y + (initialTriangle[1].pos.y - initialTriangle[0].pos.y) * 0.5f,
					1.0f
				},
				{ // Color
					initialTriangle[0].col.r + (initialTriangle[1].col.r - initialTriangle[0].col.r) * 0.5f,
					initialTriangle[0].col.g + (initialTriangle[1].col.g - initialTriangle[0].col.g) * 0.5f,
					initialTriangle[0].col.b + (initialTriangle[1].col.b - initialTriangle[0].col.b) * 0.5f
				}
			},
			{
				{ // Position
					initialTriangle[0].pos.x + (initialTriangle[2].pos.x - initialTriangle[0].pos.x) * 0.5f, 
					initialTriangle[0].pos.y + (initialTriangle[2].pos.y - initialTriangle[0].pos.y) * 0.5f,
					1.0f
				},
				{ // Color
					initialTriangle[0].col.r + (initialTriangle[2].col.r - initialTriangle[0].col.r) * 0.5f,
					initialTriangle[0].col.g + (initialTriangle[2].col.g - initialTriangle[0].col.g) * 0.5f,
					initialTriangle[0].col.b + (initialTriangle[2].col.b - initialTriangle[0].col.b) * 0.5f
				}
			}
		}, depth - 1);
	auto triangles2 = sierpinskiTriangle(
		{
			initialTriangle[1],
			{
				{ // position
					initialTriangle[1].pos.x + (initialTriangle[0].pos.x - initialTriangle[1].pos.x) * 0.5f,
					initialTriangle[1].pos.y + (initialTriangle[0].pos.y - initialTriangle[1].pos.y) * 0.5f,
					1.0f
				},
				{ // Color
					initialTriangle[1].col.r + (initialTriangle[0].col.r - initialTriangle[1].col.r) * 0.5f,
					initialTriangle[1].col.g + (initialTriangle[0].col.g - initialTriangle[1].col.g) * 0.5f,
					initialTriangle[1].col.b + (initialTriangle[0].col.b - initialTriangle[1].col.b) * 0.5f
				}
			},
			{
				{ // Position
					initialTriangle[1].pos.x + (initialTriangle[2].pos.x - initialTriangle[1].pos.x) * 0.5f,
					initialTriangle[1].pos.y + (initialTriangle[2].pos.y - initialTriangle[1].pos.y) * 0.5f,
					1.0f
				},
				{ // Color
					initialTriangle[1].col.r + (initialTriangle[2].col.r - initialTriangle[1].col.r) * 0.5f,
					initialTriangle[1].col.g + (initialTriangle[2].col.g - initialTriangle[1].col.g) * 0.5f,
					initialTriangle[1].col.b + (initialTriangle[2].col.b - initialTriangle[1].col.b) * 0.5f
				}
			}
		}, depth - 1);
	auto triangles3 = sierpinskiTriangle(
		{
			initialTriangle[2],
			{
				{ // position
					initialTriangle[2].pos.x + (initialTriangle[0].pos.x - initialTriangle[2].pos.x) * 0.5f,
					initialTriangle[2].pos.y + (initialTriangle[0].pos.y - initialTriangle[2].pos.y) * 0.5f,
					1.0f
				},
				{ // Color
					initialTriangle[2].col.r + (initialTriangle[0].col.r - initialTriangle[2].col.r) * 0.5f,
					initialTriangle[2].col.g + (initialTriangle[0].col.g - initialTriangle[2].col.g) * 0.5f,
					initialTriangle[2].col.b + (initialTriangle[0].col.b - initialTriangle[2].col.b) * 0.5f,
				}
			},
			{
				{ // Position
					initialTriangle[2].pos.x + (initialTriangle[1].pos.x - initialTriangle[2].pos.x) * 0.5f,
					initialTriangle[2].pos.y + (initialTriangle[1].pos.y - initialTriangle[2].pos.y) * 0.5f,
					1.0f
				},
				{ // Color
					initialTriangle[2].col.r + (initialTriangle[1].col.r - initialTriangle[2].col.r) * 0.5f,
					initialTriangle[2].col.g + (initialTriangle[1].col.g - initialTriangle[2].col.g) * 0.5f,
					initialTriangle[2].col.b + (initialTriangle[1].col.b - initialTriangle[2].col.b) * 0.5f
				}
			}
		}, depth - 1);

	std::vector<phm::Model::Vertex> retVec;
	retVec.insert(retVec.end(), triangles1.begin(), triangles1.end());
	retVec.insert(retVec.end(), triangles2.begin(), triangles2.end());
	retVec.insert(retVec.end(), triangles3.begin(), triangles3.end());

	return retVec;
}

namespace evo
{
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

		loadObjects();
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
		ImGui::SliderFloat2("Smooth Vase X and Y", &objects_[0].transform.translation.x, -5.0f, 5.0f);

		ImGui::End();



	}

	void Application::run()
	{
		SimpleRenderSystem simpleRenderSystem{ device_, renderer_.getSwapChainRenderPass() };

		while (!window_.shouldClose())
		{
			glfwPollEvents();

			// BeginFrame returns a nullptr if the swapchain needs to be recreated. 
			// This skips the frame draw call, if that's the case.
			auto commandBuffer = renderer_.beginFrame();
			if (commandBuffer != nullptr)
			{
				renderer_.beginSwapChainRenderPass(commandBuffer);
				// Render scene
				simpleRenderSystem.renderObjects(commandBuffer, objects_);

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


	void Application::loadObjects()
	{
		std::vector<phm::Model::Vertex> vertices = sierpinskiTriangle(
			{ { 
				{{0.0f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}},
				{{0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f}},
				{{-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}}
			} }, 6);

		/*std::vector<Model::Vertex> vertices = {
			{{0.0f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}}
		};*/
		
		auto model = std::make_shared<phm::Model>(device_, vertices);

		phm::Object obj{};
		obj.model = model;
		obj.color = { 0.7f, 0.8f,0.15f };
		obj.transform.translation.x = 0.0f;
		obj.transform.translation.y = 0.3f;
		obj.transform.scale = { 1.0f, 1.0f };
		obj.transform.setRotation(0.05f * glm::two_pi<float>());

		objects_.push_back(std::move(obj));
	}
}


