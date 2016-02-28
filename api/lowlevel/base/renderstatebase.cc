//------------------------------------------------------------------------------
// renderstatebase.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "renderstatebase.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
RenderStateBase::RenderStateBase()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
RenderStateBase::~RenderStateBase()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
RenderStateBase::OnLoaded()
{
	// empty, override in subclass
}

} // namespace AnyFX