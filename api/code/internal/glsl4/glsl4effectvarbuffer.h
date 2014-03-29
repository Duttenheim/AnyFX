#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::GLSL4EffectVarbuffer
    
    Implements a shader storage block.
    
    (C) 2014 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectvarbuffer.h"
namespace AnyFX
{
class GLSL4EffectVarbuffer : public InternalEffectVarbuffer
{
public:
	/// constructor
	GLSL4EffectVarbuffer();
	/// destructor
	virtual ~GLSL4EffectVarbuffer();
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------