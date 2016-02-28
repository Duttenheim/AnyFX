 #pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::VarblockLoader
    
    Decodes a varblock from stream.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include <EASTL/vector.h>
#include <EASTL/map.h>
#include <EASTL/string.h>
namespace AnyFX
{
class ShaderEffect;
struct VarblockBase;
struct VariableBase;
class VarblockLoader
{
public:
	/// constructor
	VarblockLoader();
	/// destructor
	virtual ~VarblockLoader();

private:
	friend class StreamLoader;

	VarblockBase* Load(BinReader* reader, ShaderEffect* effect, eastl::vector<VariableBase*>& vars);
	static eastl::map<eastl::string, VarblockBase*> sharedBlocks;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------