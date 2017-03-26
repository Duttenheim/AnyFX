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
	byteSize(0),
	binding(0),
	set(0),
	qualifiers(Qualifiers::None)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VarblockBase::~VarblockBase()
{
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		delete this->variables[i];
	}
	this->variables.clear();
	this->variablesByName.clear();
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

	// create a signature for the varblock
	this->signature.append(this->name + "{");

	// format signature by retrieving all variable signatures and making a string mask
	for (i = 0; i < this->variables.size(); i++)
	{
		VariableBase* variable = this->variables[i];
		this->signature.append(variable->signature);
		this->signature.append(";");
	}
	this->signature.append("}");
}

} // namespace AnyFX