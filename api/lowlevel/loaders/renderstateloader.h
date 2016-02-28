#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::RenderStateLoader
    
    Decodes an EffectRenderState from stream.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include "annotable.h"
namespace AnyFX
{
class ShaderEffect;
struct RenderStateBase;
class RenderStateLoader
{
public:
	/// constructor
	RenderStateLoader();
	/// destructor
	virtual ~RenderStateLoader();

private:
	friend class StreamLoader;

	RenderStateBase* Load(BinReader* reader, ShaderEffect* effect);
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------