#pragma once
//------------------------------------------------------------------------------
/**
	Implements a render state which can convert its fields to the Vulkan representatives.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vulkan/vulkan.h>
#include "internal/internaleffectrenderstate.h"

namespace AnyFX
{
class VkEffectRenderState : public InternalEffectRenderState
{
public:
	/// constructor
	VkEffectRenderState();
	/// destructor
	virtual ~VkEffectRenderState();

	/// setup rasterization info
	void SetupRasterization(VkPipelineRasterizationStateCreateInfo* info);
	/// setup multisample info
	void SetupMultisample(VkPipelineMultisampleStateCreateInfo* info);
	/// setup depth-stencil info
	void SetupDepthStencil(VkPipelineDepthStencilStateCreateInfo* info);
	/// setup blending info
	void SetupBlend(VkPipelineColorBlendStateCreateInfo* info);
private:
	VkPipelineColorBlendAttachmentState states[8];
};
} // namespace AnyFX