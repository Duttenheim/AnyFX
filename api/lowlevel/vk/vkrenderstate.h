#pragma once
//------------------------------------------------------------------------------
/**
	Contains the most basic representation of a render state.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/renderstatebase.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
struct VkRenderState : public RenderStateBase
{
public:
	/// constructor
	VkRenderState();
	/// destructor
	virtual ~VkRenderState();

	/// setup rasterization info
	void SetupRasterization(VkPipelineRasterizationStateCreateInfo* info);
	/// setup multisample info
	void SetupMultisample(VkPipelineMultisampleStateCreateInfo* info);
	/// setup depth-stencil info
	void SetupDepthStencil(VkPipelineDepthStencilStateCreateInfo* info);
	/// setup blending info
	void SetupBlend(VkPipelineColorBlendStateCreateInfo* info);

	static const uint32_t NumBlendSettings = 8;
private:

	VkPipelineColorBlendAttachmentState states[NumBlendSettings];
};
} // namespace AnyFX