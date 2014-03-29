#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectVarBufferStreamLoader
    
    Loads variable buffers from stream.
    
    (C) 2014 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "binreader.h"
namespace AnyFX
{
class Effect;
class EffectVarbuffer;
class EffectVarbufferStreamLoader
{
public:
	/// constructor
	EffectVarbufferStreamLoader();
	/// destructor
	virtual ~EffectVarbufferStreamLoader();

private:
    friend class EffectStreamLoader;

    // load variable buffer into object
    EffectVarbuffer* Load(BinReader* reader, Effect* effect);
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------