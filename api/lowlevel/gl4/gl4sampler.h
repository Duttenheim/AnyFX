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
struct GL4Sampler : public SamplerBase
{
public:
	/// constructor
	GL4Sampler();
	/// destructor
	virtual ~GL4Sampler();
private:
};
} // namespace AnyFX