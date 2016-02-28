#pragma once
//------------------------------------------------------------------------------
/**
	Describes a basic sahder object.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/shaderbase.h"
namespace AnyFX
{
struct VkShader : public ShaderBase
{
public:
	/// constructor
	VkShader();
	/// destructor
	virtual ~VkShader();
private:
};
} // namespace AnyFX