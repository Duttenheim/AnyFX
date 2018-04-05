//------------------------------------------------------------------------------
//  shaderloader.cc
//  (C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "shaderloader.h"
#include "shadereffect.h"
#include "base/shaderbase.h"
#include "gl4/gl4shader.h"
#include "effect.h"
#include "vk/vkshader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
ShaderLoader::ShaderLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderLoader::~ShaderLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderBase* 
ShaderLoader::Load(BinReader* reader, ShaderEffect* effect)
{
	ShaderBase* shader = 0;

	// we should create our implementation back-end first
	if (effect->header == Implementation::GLSL)
	{
		if (effect->major == 4) shader = new GL4Shader;
	}
	else if (effect->header == Implementation::SPIRV)
	{
		shader = new VkShader;
	}
	else
	{
		shader = new ShaderBase;
	}

	// get data
	unsigned shaderType = reader->ReadInt();
    std::string name = reader->ReadString().c_str();
    std::string code = reader->ReadString().c_str();

	shader->type = shaderType;
	shader->name = name;
	shader->sourceCode = code;

	// load compute shader stuff
	if (shader->type == ComputeShader)
    {
		shader->localSizes[0] = reader->ReadInt();
		shader->localSizes[1] = reader->ReadInt();
		shader->localSizes[2] = reader->ReadInt();
    }

	shader->OnLoaded();
	return shader;
}
} // namespace AnyFX
