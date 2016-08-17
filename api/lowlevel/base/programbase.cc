//------------------------------------------------------------------------------
// programbase.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "programbase.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
ProgramBase::ProgramBase() :
	patchSize(0),
	numVsInputs(0),
	numPsOutputs(0),
	valid(false)
{
	memset(&this->shaderBlock, 0, sizeof(this->shaderBlock));
}

//------------------------------------------------------------------------------
/**
*/
ProgramBase::~ProgramBase()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
ProgramBase::OnLoaded()
{
	// empty, override in subclass
}

} // namespace AnyFX