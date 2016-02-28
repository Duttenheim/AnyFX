#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectStreamLoader
    
    Loads an Effect by parsing the embedded FourCC codes and 
	applies the appropriate stream loader for each type of data type in the binary FX file.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include "programloader.h"
#include "variableloader.h"
#include "varblockloader.h"
#include "renderstateloader.h"
#include "samplerloader.h"
#include "shaderloader.h"
#include "varbufferloader.h"
#include "subroutineloader.h"
namespace AnyFX
{
class ShaderEffect;
class StreamLoader
{
public:
	/// constructor
	StreamLoader();
	/// destructor
	virtual ~StreamLoader();

	/// set binary reader, this should already be opened when the stream loader is run
	void SetReader(BinReader* reader);
	/// get binary reader
	BinReader* GetReader() const;

private:
	friend class EffectFactory;

	/// loads effect
	ShaderEffect* Load();

	ShaderLoader shaderLoader;
	VariableLoader variableLoader;
	VarblockLoader varblockLoader;
	RenderStateLoader renderStateLoader;
	ProgramLoader programLoader;
	SamplerLoader samplerLoader;
	VarbufferLoader varbufferLoader;
	SubroutineLoader subroutineLoader;
	BinReader* reader;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void 
StreamLoader::SetReader(BinReader* reader)
{
	this->reader = reader;
}

//------------------------------------------------------------------------------
/**
*/
inline BinReader* 
StreamLoader::GetReader() const
{
	return this->reader;
}

} // namespace AnyFX
//------------------------------------------------------------------------------