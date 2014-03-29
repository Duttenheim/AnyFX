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
	// empty
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