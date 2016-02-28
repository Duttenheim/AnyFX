#pragma once
//------------------------------------------------------------------------------
/**
	Contains the most basic representation of a render state.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/renderstatebase.h"
namespace AnyFX
{
struct GL4RenderState : public RenderStateBase
{
public:
	/// constructor
	GL4RenderState();
	/// destructor
	virtual ~GL4RenderState();
private:
};
} // namespace AnyFX