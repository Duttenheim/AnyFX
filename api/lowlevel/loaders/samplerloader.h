#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::SamplerLoader
    
    Loads sampler from binary blob
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include "shadereffect.h"
namespace AnyFX
{
class ShaderEffect;
struct SamplerBase;
class SamplerLoader
{
public:
	/// constructor
	SamplerLoader();
	/// destructor
	virtual ~SamplerLoader();

private:
	friend class StreamLoader;

	SamplerBase* Load(BinReader* reader, ShaderEffect* effect);
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------