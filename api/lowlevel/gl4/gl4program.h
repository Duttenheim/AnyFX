#pragma once
//------------------------------------------------------------------------------
/**
	Describes a base program object as written in the AnyFX shader file.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/programbase.h"
namespace AnyFX
{
struct GL4Program : public ProgramBase
{
public:
	/// constructor
	GL4Program();
	/// destructor
	virtual ~GL4Program();
private:
};
} // namespace AnyFX