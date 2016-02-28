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
	manualFlushing(false),
    bufferHandle(0),
	masterBlock(NULL)
{
	this->Retain();
}

//------------------------------------------------------------------------------
/**
*/
InternalEffectVarblock::~InternalEffectVarblock()
{
	this->childBlocks.clear();
	this->masterBlock = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalEffectVarblock::Setup(eastl::vector<InternalEffectProgram*> programs)
{
	// point our master block to ourselves, this way we can always locate the master block of any varblock
	this->masterBlock = this;
	this->bufferHandle = new void*;
    *this->bufferHandle = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalEffectVarblock::SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVarblock* master)
{
	assert(!this->isSlave);

	// set master pointer
	this->masterBlock = master;
	this->masterBlock->childBlocks.push_back(this);

	// set slave flag
	this->isSlave = true;

    // make sure slaved varblocks use the same handle
    this->bufferHandle = masterBlock->bufferHandle;

	for (unsigned i = 0; i < this->variables.size(); i++)
	{
		this->variables[i]->sharedByteOffset = this->masterBlock->variables[i]->sharedByteOffset;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InternalEffectVarblock::SetupDefaultValues()
{
    // initialized variable
    for (unsigned i = 0; i < this->variables.size(); i++)
    {
        InternalEffectVariable* variable = this->variables[i];
        variable->InitializeDefaultValues();
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
	this->signature.append(this->name + "{ ");
	// format signature by retrieving all variable signatures and making a string mask
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		InternalEffectVariable* variable = this->variables[i];
		this->signature.append(variable->GetSignature());
		this->signature.append(";");
	}
	this->signature.append(" }");
}

//------------------------------------------------------------------------------
/**
*/
void
InternalEffectVarblock::Activate(InternalEffectProgram* program)
{
	// override me!
}

} // namespace AnyFX
