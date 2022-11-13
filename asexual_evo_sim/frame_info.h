#ifndef FRAME_INFO_H
#define FRAME_INFO_H

#include <vulkan/vulkan.h>


namespace evo
{
struct FrameInfo
{
	int frameIndex;
	VkCommandBuffer commandBuffer;
	VkDescriptorSet globalDescriptorSet;
};
}

#endif /* PHM_FRAME_INFO_H */
