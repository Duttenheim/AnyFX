#pragma once
//------------------------------------------------------------------------------
/**
	Describes the basics of a variable block.

	Variable blocks are defined in GLSL/SPIR-V as a uniform buffer, and DX as a constant buffer.
	
	Provides a list of offsets per variable into the block, which is oblivious of whether or not 
	the variable is used in any shader.

	Also provides a size and aligned size, the aligned size being a layout specific padded size,
	while size is just the plain byte size. If the aligned size is primitively implemented, then 
	it will be the same as the ordinary size. 
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
#include <string>
#include <vector>
#include <map>
#include "types.h"
namespace AnyFX
{
struct VariableBase;
struct VarblockBase : public Annotable
{
public:
	/// constructor
	VarblockBase();
	/// destructor
	virtual ~VarblockBase();

	std::string name;
	std::string signature;
	unsigned byteSize;
	unsigned alignedSize;
	std::vector<VariableBase*> variables;
	std::map<std::string, VariableBase*> variablesByName;
	std::map<std::string, unsigned> offsetsByName;

	// AnyFX qualifier flags
	Qualifiers qualifiers;

	// Shader flags
	unsigned binding;	// binding means this block has a predetermined binding point within the shader code
	unsigned set;		// the update group declared in the shader (using the group(X) syntax)

protected:
	friend class VarblockLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX