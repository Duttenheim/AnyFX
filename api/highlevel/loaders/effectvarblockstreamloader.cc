//------------------------------------------------------------------------------
//  effectvarblockstreamloader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectvarblockstreamloader.h"
#include "effectvariablestreamloader.h"
#include "highlevel/effect.h"
#include "effectvarblock.h"
#include "effectvariable.h"
#include "effectprogram.h"
#include "internal/internaleffectprogram.h"
#include "internal/internaleffectvariable.h"
#include "internal/glsl4/glsl4effectvarblock.h"
#include <assert.h>
#include "effectannotationstreamloader.h"
#include "internal/vk/vkeffectvarblock.h"
#include "debug.h"

namespace AnyFX
{

eastl::map<eastl::string, InternalEffectVarblock*> EffectVarblockStreamLoader::sharedBlocks;
//------------------------------------------------------------------------------
/**
*/
EffectVarblockStreamLoader::EffectVarblockStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVarblockStreamLoader::~EffectVarblockStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVarblock*
EffectVarblockStreamLoader::Load(BinReader* reader, Effect* effect, eastl::vector<EffectVariable*>& vars)
{
	InternalEffectVarblock* internalVarblock = 0;

	// we should create our implementation back-end first
	if (effect->GetType() == Implementation::GLSL)
	{
		if (effect->GetMajor() == 4) internalVarblock = new GLSL4EffectVarblock;
	}
	else
	{
		Error("No implementation for '%d'\n", effect->GetType());
	}
	EffectVarblock* varblock = new EffectVarblock;

	eastl::string name = reader->ReadString().c_str();
	bool shared = reader->ReadBool();
	unsigned binding = reader->ReadUInt();
	unsigned set = reader->ReadUInt();
	bool push = reader->ReadBool();

    // load annotations
	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		EffectAnnotationStreamLoader loader;
		loader.Load(reader, varblock);
	}

    // load variables
	unsigned numVars = reader->ReadInt();
	EffectVariableStreamLoader variableLoader;
	unsigned i;
	for (i = 0; i < numVars; i++)
	{
		// read variable
		EffectVariable* var = variableLoader.Load(reader, effect, internalVarblock);
		internalVarblock->variables.push_back(var->internalVariable);
		internalVarblock->variablesByName[var->internalVariable->name] = var->internalVariable;
		
		// make sure to flag this variable as internal to the varblock
		vars.push_back(var);
	}

	size_t numPrograms = effect->GetNumPrograms();
	EffectProgram** programs = effect->GetPrograms();
	eastl::vector<InternalEffectProgram*> internalPrograms;
	internalPrograms.reserve(numPrograms);

	for (i = 0; i < numPrograms; i++)
	{
        if (programs[i]->IsValid()) internalPrograms.push_back(programs[i]->internalProgram);
	}

    // set internal variables
	internalVarblock->name = name;
	internalVarblock->isShared = shared;
	internalVarblock->set = set;
	internalVarblock->binding = binding;
	internalVarblock->push = push;
	internalVarblock->SetupSignature();

	if (internalVarblock->isShared)
	{
		if (sharedBlocks.find(internalVarblock->GetSignature()) != sharedBlocks.end())
		{
			// get shared block
			InternalEffectVarblock* sharedBlock = sharedBlocks[internalVarblock->GetSignature()];

			// bump reference counter of internal varblock
			sharedBlock->Retain();

			// add programs to block
			internalVarblock->SetupSlave(internalPrograms, sharedBlock);
		}
		else
		{
			// set varblock in shared list
			sharedBlocks[internalVarblock->GetSignature()] = internalVarblock;

			// setup varblock
			internalVarblock->Setup(internalPrograms);
		}
	}
	else
	{
		// setup varblock
		internalVarblock->Setup(internalPrograms);
	}	

	varblock->internalVarblock = internalVarblock;
	return varblock;
}
} // namespace AnyFX
