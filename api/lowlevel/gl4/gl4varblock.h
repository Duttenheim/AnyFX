#pragma once
//------------------------------------------------------------------------------
/**
	Describes the basics of a variable block.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/varblockbase.h"
namespace AnyFX
{
struct GL4Varblock : public VarblockBase
{
public:
	/// constructor
	GL4Varblock();
	/// destructor
	virtual ~GL4Varblock();
private:
};
} // namespace AnyFX