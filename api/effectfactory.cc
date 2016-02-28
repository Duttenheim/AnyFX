//------------------------------------------------------------------------------
//  effectfactory.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectfactory.h"
#include <assert.h>
#include "binreader.h"
#include <fstream>

namespace AnyFX
{

EffectFactory* EffectFactory::instance = 0;
//------------------------------------------------------------------------------
/**
*/
EffectFactory::EffectFactory()
{
	assert(instance == 0);
	instance = this;
}

//------------------------------------------------------------------------------
/**
*/
EffectFactory::~EffectFactory()
{
	assert(instance != 0);
	instance = 0;
}

//------------------------------------------------------------------------------
/**
*/
EffectFactory* 
EffectFactory::Instance()
{
	assert(instance != 0);
	return instance;
}


//------------------------------------------------------------------------------
/**
*/
Effect* 
EffectFactory::CreateEffectFromFile(const eastl::string& file)
{
	EffectStreamLoader loader;
	BinReader reader;
	reader.SetPath(file.c_str());
	reader.Open();
	loader.SetReader(&reader);
	Effect* retval = loader.Load();
	loader.SetReader(0);
	reader.Close();
	return retval;
}

//------------------------------------------------------------------------------
/**
*/
Effect*
EffectFactory::CreateEffectFromMemory(void* data, size_t size)
{
	EffectStreamLoader loader;
	BinReader* reader = new BinReader;
	reader->Open((const char*)data, size);
	loader.SetReader(reader);
	Effect* retval = loader.Load();
	loader.SetReader(0);
	reader->Close();	
	delete reader;
	return retval;
}

//------------------------------------------------------------------------------
/**
*/
ShaderEffect*
EffectFactory::CreateShaderEffectFromFile(const eastl::string& file)
{
	StreamLoader loader;
	BinReader reader;
	reader.SetPath(file.c_str());
	reader.Open();
	loader.SetReader(&reader);
	ShaderEffect* retval = loader.Load();
	loader.SetReader(0);
	reader.Close();
	return retval;
}

//------------------------------------------------------------------------------
/**
*/
ShaderEffect*
EffectFactory::CreateShaderEffectFromMemory(void* data, size_t size)
{
	StreamLoader loader;
	BinReader* reader = new BinReader;
	reader->Open((const char*)data, size);
	loader.SetReader(reader);
	ShaderEffect* retval = loader.Load();
	loader.SetReader(0);
	reader->Close();
	delete reader;
	return retval;
}

} // namespace AnyFX