#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectProgramStreamLoader
    
    Decodes an EffectProgram from the input stream.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include "annotable.h"
namespace AnyFX
{
class ShaderEffect;
struct ProgramBase;
class ProgramLoader
{
public:
	/// constructor
	ProgramLoader();
	/// destructor
	virtual ~ProgramLoader();

private:
	friend class StreamLoader;

	ProgramBase* Load(BinReader* reader, ShaderEffect* effect);
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------