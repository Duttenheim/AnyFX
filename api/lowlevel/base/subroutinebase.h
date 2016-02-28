#pragma once
//------------------------------------------------------------------------------
/**
	Encapsulates a subroutine.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
namespace AnyFX
{
struct SubroutineBase : public Annotable
{
public:
	/// constructor
	SubroutineBase();
	/// destructor
	virtual ~SubroutineBase();

	eastl::string name;
	unsigned type;

private:
	friend class SubroutineLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX