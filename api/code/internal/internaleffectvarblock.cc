//------------------------------------------------------------------------------
//  internaleffectvarblock.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "internaleffectvarblock.h"
#include "internaleffectvariable.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
InternalEffectVarblock::InternalEffectVarblock() :
	active(false),
	isShared(false),
	isDirty(true),
	isSlave(false),
	masterBlock(NULL),
	refCount(1)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
InternalEffectVarblock::~InternalEffectVarblock()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarblock::Setup( std::vector<InternalEffectProgram*> programs )
{
	// create new datablock if and only if we are the master varblock
	this->dataBlock = new InternalVarblockData;

	// set size to 0
	this->dataBlock->size = 0;

	// setup variable offsets and sizes
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		InternalEffectVariable* variable = this->variables[i];
		variable->byteOffset = this->dataBlock->size;
		this->dataBlock->size += variable->byteSize;
	}

	// create cpu-side buffer
	this->dataBlock->data = new char[this->dataBlock->size];

	// initialized variable
	for (i = 0; i < this->variables.size(); i++)
	{
		InternalEffectVariable* variable = this->variables[i];
		variable->InitializeDefaultValues();
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarblock::SetupSlave( std::vector<InternalEffectProgram*> programs, InternalEffectVarblock* master )
{
	assert(!this->isSlave);

	// copy pointer to data buffer
	this->dataBlock = master->dataBlock;

	// set master pointer
	this->masterBlock = master;

	// set slave flag
	this->isSlave = true;

	// calculate size again
	unsigned size = 0;

	// initialized variable
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		InternalEffectVariable* variable = this->variables[i];
		variable->byteOffset = size;
		size += variable->byteSize;
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarblock::Apply()
{
	// override me!
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarblock::Commit()
{
	// override me!
}

//------------------------------------------------------------------------------
/**
	Run this when varblock is properly setup
*/
void 
InternalEffectVarblock::SetupSignature()
{
	// format signature by retrieving all variable signatures and making a string mask
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		InternalEffectVariable* variable = this->variables[i];
		this->signature.append(variable->GetSignature());
		this->signature.append(";");
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarblock::SetVariable( InternalEffectVariable* var, void* value )
{
	memcpy((void*)(this->dataBlock->data + var->byteOffset), value, var->byteSize);
	this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
	Since arrays can be incrementally updated
*/
void 
InternalEffectVarblock::SetVariableArray( InternalEffectVariable* var, void* value, size_t size )
{
	memcpy((void*)(this->dataBlock->data + var->byteOffset), value, size);
	this->isDirty = true;	
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarblock::Activate( InternalEffectProgram* program )
{
	// override me!
}

} // namespace AnyFX
