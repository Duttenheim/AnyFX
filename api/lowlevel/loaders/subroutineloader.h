#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectSubroutineStreamLoader
    
    Loads a subroutine from stream.
    
    (C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include "shadereffect.h"
namespace AnyFX
{
class ShaderEffect;
struct SubroutineBase;
class SubroutineLoader
{
public:
	/// constructor
	SubroutineLoader();
	/// destructor
	virtual ~SubroutineLoader();

private:
    friend class StreamLoader;

    // load subroutine into object
    SubroutineBase* Load(BinReader* reader, ShaderEffect* effect);
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------