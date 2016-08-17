#pragma once
//------------------------------------------------------------------------------
/**
	Describes a basic sampler object.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/samplerbase.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
struct VkSampler : public SamplerBase
{
public:
	/// constructor
	VkSampler();
	/// destructor
	virtual ~VkSampler();

	VkSamplerCreateInfo samplerInfo;
	VkDescriptorSetLayoutBinding bindingLayout;
private:

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX