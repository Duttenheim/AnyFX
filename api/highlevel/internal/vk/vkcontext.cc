//------------------------------------------------------------------------------
// vkcontext.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkcontext.h"

namespace AnyFX
{

VkDevice VkContext::currentContext;
VkPipelineCache VkContext::currentCache;
VkCommandBuffer VkContext::currentCommandBuffer;
VkDescriptorPool VkContext::currentPool;
uint32_t VkContext::descriptorSetRingSize = 127;

} // namespace AnyFX
