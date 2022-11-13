#ifndef PHM_SIMPLE_RENDER_SYSTEM_H
#define PHM_SIMPLE_RENDER_SYSTEM_H

#include <memory>
#include <vector>

#include "phm_pipeline.h"
#include "phm_object.h"


namespace evo
{
	class SimpleRenderSystem
	{

	public:
		SimpleRenderSystem(phm::Device& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderObjects(VkCommandBuffer commandBuffer, const std::vector<phm::Object>& objects);

	private:
		phm::Device& device_;

		std::unique_ptr<phm::Pipeline> pipeline_;
		VkPipelineLayout pipelineLayout_;

		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
	};
}

#endif /* PHM_SIMPLE_RENDER_SYSTEM_H */

