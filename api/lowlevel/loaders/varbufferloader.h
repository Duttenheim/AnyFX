#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectVarBufferStreamLoader
    
    Loads variable buffers from stream.
    
    (C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include <vector>
#include <map>
#include <string>
namespace AnyFX
{
class ShaderEffect;
struct VarbufferBase;
class VarbufferLoader
{
public:
	/// constructor
	VarbufferLoader();
	/// destructor
	virtual ~VarbufferLoader();

private:
    friend class StreamLoader;

    // load variable buffer into object
	VarbufferBase* Load(BinReader* reader, ShaderEffect* effect);
	static std::map<std::string, VarbufferBase*> sharedBuffers;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------