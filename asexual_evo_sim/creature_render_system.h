#ifndef CREATURE_RENDER_SYSTEM_H
#define CREATURE_RENDER_SYSTEM_H

#include <memory>
#include <vector>

#include "phm_pipeline.h"
#include "phm_object.h"

#include "simulator.h"
#include "frame_info.h"


namespace evo
{
	class CreatureRenderSystem
	{

	public:
		CreatureRenderSystem(phm::Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~CreatureRenderSystem();

		CreatureRenderSystem(const CreatureRenderSystem&) = delete;
		CreatureRenderSystem& operator=(const CreatureRenderSystem&) = delete;

		void renderObjects(const FrameInfo& frameInfo, const std::vector<Simulator::Creature>& objects);

	private:
		phm::Device& device_;

		std::unique_ptr<phm::Pipeline> pipeline_;
		VkPipelineLayout pipelineLayout_;

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
}

#endif /* CREATURE_RENDER_SYSTEM_H */

