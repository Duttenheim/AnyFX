#pragma once
//------------------------------------------------------------------------------
/**
	Describes the basics of a variable block.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>
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

	eastl::string name;
	eastl::string signature;
	unsigned byteSize;
	eastl::vector<VariableBase*> variables;
	eastl::map<eastl::string, VariableBase*> variablesByName;

	// AnyFX managed flags
	bool isShared;		// shared means this varblock will be identical to all others with the same name in terms of layout, size and variables

	// Shader flags
	unsigned binding;	// binding means this block has a predetermined binding point within the shader code
	unsigned set;		// used for DX12 and Vulkan, describes the descriptor set binding slot
	bool push;			// used for DX12 and Vulkan, describes if this block is meant as a transient micro buffer for rapid uniform updates.

protected:
	friend class VarblockLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();

	/// creates signature which is used for shared varblocks
	void SetupSignature();
};
} // namespace AnyFX