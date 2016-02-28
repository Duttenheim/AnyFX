#pragma once
//------------------------------------------------------------------------------
/**
	Implements a basic variable buffer.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/varbufferbase.h"
namespace AnyFX
{
struct GL4Varbuffer : public VarbufferBase
{
public:
	/// constructor
	GL4Varbuffer();
	/// destructor
	virtual ~GL4Varbuffer();
private:
};
} // namespace AnyFX