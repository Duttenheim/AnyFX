//------------------------------------------------------------------------------
// varbufferbase.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "varbufferbase.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VarbufferBase::VarbufferBase() :
	binding(0),
	set(0),
	qualifiers(Qualifiers::None)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VarbufferBase::~VarbufferBase()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VarbufferBase::OnLoaded()
{
	this->signature = this->name;
}

} // namespace AnyFX