#pragma once
//------------------------------------------------------------------------------
/**
	Implements a basic variable buffer.

	Variable buffers are defined in GLSL/SPIR-V as a shader storage buffer, and DX as a RWStructuredBuffer.

	Provides a list of offsets per variable into the buffer, which is oblivious of whether or not
	the variable is used in any shader, although variable buffers are usually directly mappable from CPU memory.

	Also provides a size and aligned size, the aligned size being a layout specific padded size,
	while size is just the plain byte size. If the aligned size is primitively implemented, then
	it will be the same as the ordinary size.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
#include "types.h"
namespace AnyFX
{
struct VarbufferBase : public Annotable
{
public:
	/// constructor
	VarbufferBase();
	/// destructor
	virtual ~VarbufferBase();

	std::string name;
	std::string signature;
	unsigned alignedSize;
	unsigned size;
	bool active;
	std::map<std::string, unsigned> offsetsByName;

	// AnyFX qualifier flags
	Qualifiers qualifiers;

	unsigned binding;
	unsigned set;

protected:
	friend class VarbufferLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX