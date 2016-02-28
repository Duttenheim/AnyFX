#pragma once
//------------------------------------------------------------------------------
/**
	Describes the base for a variable.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/variablebase.h"
namespace AnyFX
{
struct GL4Variable : public VariableBase
{
public:
	/// constructor
	GL4Variable();
	/// destructor
	virtual ~GL4Variable();
private:
};
} // namespace AnyFX