#pragma once
//------------------------------------------------------------------------------
/**
	Implements a Vulkan sampler state.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectsampler.h"
#include <vulkan/vulkan.h>

namespace AnyFX
{
class VkEffectSampler : public InternalEffectSampler
{
public:
	/// constructor
	VkEffectSampler();
	/// destructor
	virtual ~VkEffectSampler();

private:

	/// setup using texture variable
	virtual void Setup(const eastl::vector<InternalEffectVariable*>& textures);

	VkSampler sampler;
};
} // namespace AnyFX