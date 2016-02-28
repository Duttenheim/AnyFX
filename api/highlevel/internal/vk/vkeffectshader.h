#pragma once
//------------------------------------------------------------------------------
/**
	Implements a Vulkan shader, but is only relevant for program creation.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectshader.h"
#include <vulkan/vulkan.h>

namespace AnyFX
{
class VkEffectShader : public InternalEffectShader
{
public:
	/// constructor
	VkEffectShader();
	/// destructor
	virtual ~VkEffectShader();
private:
	friend class VkEffectProgram;

	/// compiles shader
	bool Compile();

	VkShaderModule shader;
};
} // namespace AnyFX