//------------------------------------------------------------------------------
//  glsl4effectsampler.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "glsl4effectsampler.h"
#include "glsl4effectvariable.h"
#include <assert.h>

namespace AnyFX
{

static const GLenum opengl4AddressModeTable[] =
{
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRRORED_REPEAT
};

static const GLenum opengl4ComparisonTable[] = 
{
	GL_NEVER,
	GL_LESS,
	GL_LEQUAL,
	GL_GREATER,
	GL_GEQUAL,
	GL_EQUAL,
	GL_NOTEQUAL,	
	GL_ALWAYS
};

static const GLenum opengl4MinMipTable[] = 
{
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_LINEAR,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_LINEAR,
	GL_NEAREST
};

static const GLenum opengl4MagTable[] = 
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST,
	GL_LINEAR,
	GL_LINEAR,
	GL_NEAREST,
	GL_NEAREST,
	GL_LINEAR,
	GL_LINEAR,
	GL_NEAREST,
};

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectSampler::GLSL4EffectSampler() :
	sampler(0),
	textures(0),
	numTextures(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectSampler::~GLSL4EffectSampler()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectSampler::Setup( const std::vector<InternalEffectVariable*>& textures )
{
	InternalEffectSampler::Setup(textures);

	// get number of textures and allocate array for all textures
	this->numTextures = textures.size();
	this->textures = new GLuint[this->numTextures];
	
	unsigned i;
	for (i = 0; i < textures.size(); i++)
	{
		GLSL4EffectVariable* texVar = dynamic_cast<GLSL4EffectVariable*>(textures[i]);
		assert(0 != texVar);

		// get texture slot from variable
		this->textures[i] = texVar->textureUnit;
	}	

	// generate sampler object
	glGenSamplers(1, &this->sampler);

	// setup parameters
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_S, opengl4AddressModeTable[this->samplerSettings.addressU]);
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_T, opengl4AddressModeTable[this->samplerSettings.addressV]);
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_R, opengl4AddressModeTable[this->samplerSettings.addressW]);
	glSamplerParameteri(this->sampler, GL_TEXTURE_MIN_FILTER, opengl4MinMipTable[this->samplerSettings.filterMode]);
	glSamplerParameteri(this->sampler, GL_TEXTURE_MAG_FILTER, opengl4MagTable[this->samplerSettings.filterMode]);

	// set comparison mode if this is a comparison sampler
	if (this->samplerSettings.isComparison)
	{
		glSamplerParameteri(this->sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(this->sampler, GL_TEXTURE_COMPARE_FUNC, opengl4ComparisonTable[this->samplerSettings.comparisonFunc]);
	}
	else
	{
		glSamplerParameteri(this->sampler, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glSamplerParameteri(this->sampler, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);
	}

	glSamplerParameterf(this->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->samplerSettings.maxAnisotropic);
	glSamplerParameterf(this->sampler, GL_TEXTURE_LOD_BIAS, this->samplerSettings.lodBias);
	glSamplerParameterf(this->sampler, GL_TEXTURE_MIN_LOD, this->samplerSettings.minLod);
	glSamplerParameterf(this->sampler, GL_TEXTURE_MAX_LOD, this->samplerSettings.maxLod);
	glSamplerParameterfv(this->sampler, GL_TEXTURE_BORDER_COLOR, this->samplerSettings.borderColor);
}

//------------------------------------------------------------------------------
/**
*/
void
GLSL4EffectSampler::Apply()
{
	// assign sampler to all texture slots
	unsigned i;
	for (i = 0; i < this->numTextures; i++)
	{
		// bind sampler
		glBindSampler(this->textures[i], this->sampler);
	}	
}
} // namespace AnyFX