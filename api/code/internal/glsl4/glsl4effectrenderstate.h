#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::GLSL4EffectRenderState
    
    GLSL4 backend for EffectRenderState, implements InternalEffectRenderState.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectrenderstate.h"
#include "GL/glew.h"
namespace AnyFX
{
class GLSL4EffectRenderState : public InternalEffectRenderState
{
public:
	/// constructor
	GLSL4EffectRenderState();
	/// destructor
	virtual ~GLSL4EffectRenderState();

private:
	
	/// applies opengl4 render state
	void Apply();


}; 
} // namespace AnyFX
//------------------------------------------------------------------------------