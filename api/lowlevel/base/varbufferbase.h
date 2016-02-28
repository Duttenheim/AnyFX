#pragma once
//------------------------------------------------------------------------------
/**
	Implements a basic variable buffer.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
namespace AnyFX
{
struct VarbufferBase : public Annotable
{
public:
	/// constructor
	VarbufferBase();
	/// destructor
	virtual ~VarbufferBase();

	eastl::string name;
	unsigned size;
	bool isShared;
	bool active;

	unsigned binding;
	unsigned set;

private:
	friend class VarbufferLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX