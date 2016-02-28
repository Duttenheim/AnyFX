#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::ShaderLoader
    
    Low-level shader loader.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
namespace AnyFX
{
class ShaderEffect;
struct ShaderBase;
class ShaderLoader
{
public:
	/// constructor
	ShaderLoader();
	/// destructor
	virtual ~ShaderLoader();

private:
	friend class StreamLoader;

	ShaderBase* Load(BinReader* reader, ShaderEffect* effect);
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------