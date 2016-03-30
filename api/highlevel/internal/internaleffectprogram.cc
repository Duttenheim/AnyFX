//------------------------------------------------------------------------------
//  internaleffectprogram.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "internaleffectprogram.h"
#include "highlevel/effect.h"
#include "effectvariable.h"
#include "effectvarblock.h"
#include "effectvarbuffer.h"
#include "internaleffectvariable.h"
#include "internaleffectsampler.h"
#include "internaleffectvarblock.h"
#include "internaleffectsubroutine.h"
#include "internaleffectvarbuffer.h"
#include "internaleffectrenderstate.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
InternalEffectProgram::InternalEffectProgram() :
	renderState(NULL),
	linkState(NotLinked),
	supportsTessellation(false),
	patchSize(0),
	numVsInputs(0),
	numPsOutputs(0),
	numVariables(0),
	numVarblocks(0),
	numVarbuffers(0),
	numSamplers(0),
	internalHandle(NULL)
{
	this->shaderBlock.vs = NULL;
	this->shaderBlock.vsBinarySize = 0;
	this->shaderBlock.vsBinary = NULL;
	this->shaderBlock.ps = NULL;
	this->shaderBlock.psBinarySize = 0;
	this->shaderBlock.psBinary = NULL;
	this->shaderBlock.hs = NULL;
	this->shaderBlock.hsBinarySize = 0;
	this->shaderBlock.hsBinary = NULL;
	this->shaderBlock.ds = NULL;
	this->shaderBlock.dsBinarySize = 0;
	this->shaderBlock.dsBinary = NULL;
	this->shaderBlock.gs = NULL;
	this->shaderBlock.gsBinarySize = 0;
	this->shaderBlock.gsBinary = NULL;
	this->shaderBlock.cs = NULL;
	this->shaderBlock.csBinarySize = 0;
	this->shaderBlock.csBinary = NULL;
	this->variables = NULL;
	this->varblocks = NULL;
	this->varbuffers = NULL;
	this->samplers = NULL;
}

//------------------------------------------------------------------------------
/**
*/
InternalEffectProgram::~InternalEffectProgram()
{
	if (this->shaderBlock.vsBinary) delete[] this->shaderBlock.vsBinary;
	if (this->shaderBlock.psBinary) delete[] this->shaderBlock.psBinary;
	if (this->shaderBlock.hsBinary) delete[] this->shaderBlock.hsBinary;
	if (this->shaderBlock.dsBinary) delete[] this->shaderBlock.dsBinary;
	if (this->shaderBlock.gsBinary) delete[] this->shaderBlock.gsBinary;
	if (this->shaderBlock.csBinary) delete[] this->shaderBlock.csBinary;
}

//------------------------------------------------------------------------------
/**
	Hmm, maybe save the internal variables directly, this way we skip tons of pointer indirections
*/
void 
InternalEffectProgram::Apply()
{
	// signal our variables and varblocks that this program is active so that they may select their correct internal handle
	unsigned i;
	size_t num = this->numVariables;
	for (i = 0; i < num; ++i)
	{
		this->variables[i]->Activate(this);
		this->variables[i]->Apply();
	}

	num = this->numVarblocks;
	for (i = 0; i < num; ++i)
	{
		this->varblocks[i]->Activate(this);
		this->varblocks[i]->Apply();
	}

	num = this->numVarbuffers;
	for (i = 0; i < num; ++i)
	{
		this->varbuffers[i]->Activate(this);
		this->varbuffers[i]->Apply();
	}

	num = this->numSamplers;
	for (i = 0; i < num; ++i)
	{
		this->samplers[i]->Apply();
	}

	// finally apply render state
	this->renderState->Apply();
}

//------------------------------------------------------------------------------
/**
Hmm, maybe save the internal variables directly, this way we skip tons of pointer indirections
*/
void 
InternalEffectProgram::Commit()
{
	// signal our variables and varblocks to apply their variables
	unsigned i;
	size_t num = this->numVariables;
	for (i = 0; i < num; ++i)
	{
		this->variables[i]->Commit();
	}

	num = this->numVarblocks;
	for (i = 0; i < num; ++i)
	{
		this->varblocks[i]->Commit();
	}

	num = this->numVarbuffers;
	for (i = 0; i < num; ++i)
	{
		this->varbuffers[i]->Commit();
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
InternalEffectProgram::SetupSlave(InternalEffectProgram* other)
{
	// override me!
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectProgram::SetupSubroutines()
{
    // override me!
}

//------------------------------------------------------------------------------
/**
	Setup our local internal variables so that we don't have go to through the effect to get them.
	This might improve our caching later on, since variables, varblocks, varbuffers and samplers are allocated in sequence.
*/
void
InternalEffectProgram::LoadingDone()
{
	// start by setting up variables
	this->numVariables = this->effect->numVariables;
	this->variables = new InternalEffectVariable*[this->numVariables];

	unsigned i;
	for (i = 0; i < this->effect->numVariables; i++)
	{
		this->variables[i] = this->effect->variablesByIndex[i]->internalVariable;
	}

	// varblocks
	this->numVarblocks = this->effect->numVarblocks;
	this->varblocks = new InternalEffectVarblock*[this->numVarblocks];

	for (i = 0; i < this->effect->numVarblocks; i++)
	{
		this->varblocks[i] = this->effect->varblocksByIndex[i]->internalVarblock;
	}

	// varbuffers
	this->numVarbuffers = this->effect->numVarbuffers;
	this->varbuffers = new InternalEffectVarbuffer*[this->numVarbuffers];

	for (i = 0; i < this->effect->numVarbuffers; i++)
	{
		this->varbuffers[i] = this->effect->varbuffersByIndex[i]->internalVarbuffer;
	}

	// samplers
	this->numSamplers = this->effect->numSamplers;
	this->samplers = new InternalEffectSampler*[this->numSamplers];

	for (i = 0; i < this->effect->numSamplers; i++)
	{
		this->samplers[i] = this->effect->samplersByIndex[i]->internalSampler;
	}
}

} // namespace AnyFX