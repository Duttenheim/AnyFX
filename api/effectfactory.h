#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectFactory
    
    Creates Effects by using the stream loaders.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "EASTL/map.h"
#include "EASTL/string.h"
#include "highlevel/loaders/effectstreamloader.h"
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

	/// creates a high-level effect from file
	Effect* CreateEffectFromFile(const eastl::string& file);
	/// creates a high-level effect from memory
	Effect* CreateEffectFromMemory(void* data, size_t size);

	/// creates a low-level effect from file
	ShaderEffect* CreateShaderEffectFromFile(const eastl::string& file);
	/// creates a low-level effect from memory
	ShaderEffect* CreateShaderEffectFromMemory(void* data, size_t size);

private:
	
	static EffectFactory* instance;

	EffectStreamLoader loader;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------
