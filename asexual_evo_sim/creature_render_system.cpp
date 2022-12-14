#include "pch.h"

#include "creature_render_system.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <iostream>


namespace evo
{
	struct SimplePushConstantData
	{
		glm::vec4 position;
		glm::mat4 transform{ 1.0f };
		alignas(16) glm::vec3 color;
	};

	CreatureRenderSystem::CreatureRenderSystem(phm::Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		: device_(device)
	{
		createPipelineLayout(globalSetLayout);
		createPipeline(renderPass);
	}

	CreatureRenderSystem::~CreatureRenderSystem()
	{
		vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
	}

	// Self documenting
	void CreatureRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout. ");
		}
	}

	void CreatureRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(
			pipelineLayout_ != nullptr &&
			"Cannot create pipeline before the pipeline layout"
		);

		phm::PipelineConfigInfo pipelineConfig{};
		phm::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.attributeDescriptions.clear();
		pipelineConfig.bindingDescriptions.clear();

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout_;

		pipeline_ = std::make_unique<phm::Pipeline>(
			device_,
			"shaders/creature_shader.vert.spv",
			"shaders/creature_shader.frag.spv",
			pipelineConfig
			);

	}

	void CreatureRenderSystem::renderObjects(const FrameInfo& frameInfo, const std::vector<Simulator::Creature>& objects)
	{
		pipeline_->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout_,
			0,
			1,
			&frameInfo.globalDescriptorSet,
			0,
			nullptr
		);

		for (auto& obj : objects)
		{
			SimplePushConstantData push{};
			//push.offset = obj.transform.translation;
			push.color = { 0.0f, 0.4f, 0.3f };
			push.transform = obj.mat4();
			push.position = glm::vec4(obj.position, 0, 0);

			vkCmdPushConstants(
				frameInfo.commandBuffer,
				pipelineLayout_,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);

			vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
		}
	}
}
