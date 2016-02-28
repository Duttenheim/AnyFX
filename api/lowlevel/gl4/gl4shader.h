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
struct GL4Shader : public ShaderBase
{
public:
	/// constructor
	GL4Shader();
	/// destructor
	virtual ~GL4Shader();
private:
};
} // namespace AnyFX