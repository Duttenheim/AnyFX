//------------------------------------------------------------------------------
//  internaleffectprogram.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "internaleffectprogram.h"
#include "effect.h"
#include "effectvariable.h"
#include "effectvarblock.h"
#include "internaleffectvariable.h"
#include "internaleffectsampler.h"
#include "internaleffectvarblock.h"
#include "internaleffectsubroutine.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
InternalEffectProgram::InternalEffectProgram() :
	renderState(NULL),
	linkState(NotLinked),
	supportsTessellation(false),
	patchSize(0)
{
	this->shaderBlock.vs = NULL;
	this->shaderBlock.ps = NULL;
	this->shaderBlock.hs = NULL;
	this->shaderBlock.ds = NULL;
	this->shaderBlock.gs = NULL;
	this->shaderBlock.cs = NULL;
}

//------------------------------------------------------------------------------
/**
*/
InternalEffectProgram::~InternalEffectProgram()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectProgram::Apply()
{
	// signal our variables and varblocks that this program is active so that they may select their correct internal handle
	unsigned i;
	size_t num = this->effect->numVariables;
	for (i = 0; i < num; i++)
	{
		this->effect->variablesByIndex[i]->internalVariable->Activate(this);
		this->effect->variablesByIndex[i]->internalVariable->Apply();
	}

	num = this->effect->numVarblocks;
	for (i = 0; i < num; i++)
	{
		this->effect->varblocksByIndex[i]->internalVarblock->Activate(this);
		this->effect->varblocksByIndex[i]->internalVarblock->Apply();
	}

	num = this->effect->numSamplers;
	for (i = 0; i < num; i++)
	{
		EffectSampler* sampler = this->effect->samplersByIndex[i];
		sampler->internalSampler->Apply();
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectProgram::Commit()
{
	// signal our variables and varblocks to apply their variables
	unsigned i;
	size_t num = this->effect->numVariables;
	for (i = 0; i < num; i++)
	{
		EffectVariable* var = this->effect->variablesByIndex[i];
		var->internalVariable->Commit();
	}

	num = this->effect->numVarblocks;
	for (i = 0; i < num; i++)
	{
		EffectVarblock* block = this->effect->varblocksByIndex[i];
		block->internalVarblock->Commit();
	}
}

//------------------------------------------------------------------------------
/**
*/
bool 
InternalEffectProgram::Link()
{
	// override me if needed!
	return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectProgram::SetupSubroutines()
{
    // override me!
}
} // namespace AnyFX