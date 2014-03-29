//------------------------------------------------------------------------------
//  effectshader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "./effectshader.h"
#include <assert.h>
#include "internal/internaleffectshader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectShader::EffectShader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectShader::~EffectShader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectShader::Discard()
{
	delete this->internalShader;
	this->internalShader = 0;
}

//------------------------------------------------------------------------------
/**
*/
const std::string& 
EffectShader::GetError() const
{
	return this->internalShader->GetError();
}

//------------------------------------------------------------------------------
/**
*/
const std::string& 
EffectShader::GetName() const
{
	return this->internalShader->GetName();
}

//------------------------------------------------------------------------------
/**
*/
const std::string& 
EffectShader::GetCode() const
{
	return this->internalShader->GetCode();
}
} // namespace AnyFX
