#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectFactory
    
    Creates Effects by using the stream loaders.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <map>
#include <string>
#include "lowlevel/loaders/streamloader.h"
namespace AnyFX
{
class Effect;
class EffectFactory
{
public:
	/// constructor
	EffectFactory();
	/// destructor
	virtual ~EffectFactory();

	/// returns instance to factory
	static EffectFactory* Instance();

	/// creates a low-level effect from file
	ShaderEffect* CreateShaderEffectFromFile(const std::string& file);
	/// creates a low-level effect from memory
	ShaderEffect* CreateShaderEffectFromMemory(void* data, size_t size);

private:
	
	static EffectFactory* instance;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------
