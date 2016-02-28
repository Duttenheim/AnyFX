//------------------------------------------------------------------------------
// varblockbase.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "varblockbase.h"
#include "variablebase.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VarblockBase::VarblockBase() :
	byteSize(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VarblockBase::~VarblockBase()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VarblockBase::OnLoaded()
{
	// setup this varblock
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		this->byteSize += this->variables[i]->byteSize;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
VarblockBase::SetupSignature()
{
	this->signature.append(this->name + "{ ");
	// format signature by retrieving all variable signatures and making a string mask
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		VariableBase* variable = this->variables[i];
		this->signature.append(variable->signature);
		this->signature.append(";");
	}
	this->signature.append(" }");
}

} // namespace AnyFX