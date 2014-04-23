//------------------------------------------------------------------------------
//  internaleffectshader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "internaleffectshader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
InternalEffectShader::InternalEffectShader()
{
	memset(this->localSizes, 1, 3);
}

//------------------------------------------------------------------------------
/**
*/
InternalEffectShader::~InternalEffectShader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
InternalEffectShader::Compile()
{	
	// override me!
	return false;
}
} // namespace AnyFX