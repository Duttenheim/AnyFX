//------------------------------------------------------------------------------
//  effectshaderstreamloader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectshaderstreamloader.h"
#include "internal/internaleffectshader.h"
#include "internal/glsl4/glsl4effectshader.h"
#include "highlevel/effect.h"
#include "internal/vk/vkeffectshader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectShaderStreamLoader::EffectShaderStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectShaderStreamLoader::~EffectShaderStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectShader*
EffectShaderStreamLoader::Load(BinReader* reader, Effect* effect)
{
	InternalEffectShader* internalShader = 0;

	// we should create our implementation back-end first
	if (effect->GetType() == Implementation::GLSL)
	{
		if (effect->GetMajor() == 4) internalShader = new GLSL4EffectShader;
	}
	else
	{
		Error("No implementation for '%d'\n", effect->GetType());
	}
	internalShader->effect = effect;

	// get data
	ShaderType shaderType = (ShaderType)reader->ReadInt();
    eastl::string name = reader->ReadString().c_str();
    eastl::string code = reader->ReadString().c_str();

	internalShader->type = (ShaderType)shaderType;
	internalShader->name = name;
	internalShader->sourceCode = code;
	bool success = internalShader->Compile();

	// load compute shader stuff
    if (internalShader->type == ComputeShader)
    {
        internalShader->localSizes[0] = reader->ReadInt();
        internalShader->localSizes[1] = reader->ReadInt();
        internalShader->localSizes[2] = reader->ReadInt();
    }

	if (success)
	{
		EffectShader* shader = new EffectShader;
		shader->internalShader = internalShader;
		return shader;
	}
	else
	{
		delete internalShader;
		return NULL;
	}	
}
} // namespace AnyFX
