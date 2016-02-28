#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::VariableLoader
    
    Decodes a variable from stream.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include "EASTL/map.h"
#include "EASTL/string.h"

namespace AnyFX
{
class ShaderEffect;
struct VariableBase;
struct VarblockBase;
class VariableLoader
{
public:
	/// constructor
	VariableLoader();
	/// destructor
	virtual ~VariableLoader();

private:
	friend class StreamLoader;
	friend class VarblockLoader;

	VariableBase* Load(BinReader* reader, ShaderEffect* effect, VarblockBase* varblock = 0);
	static eastl::map<eastl::string, VariableBase*> sharedVariables;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------