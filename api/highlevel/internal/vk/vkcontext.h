#pragma once
//------------------------------------------------------------------------------
/**
	This class serves as a container for the currently used Vulkan device, 
	used by the system which uses AnyFX
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vulkan/vulkan.h>

namespace AnyFX
{

class VkContext
{
public:
	static VkDevice currentContext;
	static VkPipelineCache currentCache;
	static VkCommandBuffer currentCommandBuffer;
	static VkDescriptorPool currentPool;
	static uint32_t descriptorSetRingSize;
};

} // namespace AnyFX