 #pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::VarblockLoader
    
    Decodes a varblock from stream.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include <vector>
#include <map>
#include <string>
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

	VarblockBase* Load(BinReader* reader, ShaderEffect* effect, std::vector<VariableBase*>& vars);
	static std::map<std::string, VarblockBase*> sharedBlocks;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------