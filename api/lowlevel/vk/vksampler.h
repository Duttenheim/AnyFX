#pragma once
//------------------------------------------------------------------------------
/**
	Describes a basic sampler object.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/samplerbase.h"
namespace AnyFX
{
struct VkSampler : public SamplerBase
{
public:
	/// constructor
	VkSampler();
	/// destructor
	virtual ~VkSampler();
private:
};
} // namespace AnyFX