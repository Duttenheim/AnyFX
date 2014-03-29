//------------------------------------------------------------------------------
//  effectvariablestreamloader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectvariablestreamloader.h"
#include "internal/internaleffectvariable.h"
#include "effectvariable.h"
#include "effectprogram.h"
#include "effect.h"
#include "internal/glsl4/glsl4effectvariable.h"
#include "effectannotationstreamloader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectVariableStreamLoader::EffectVariableStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVariableStreamLoader::~EffectVariableStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVariable* 
EffectVariableStreamLoader::Load( BinReader* reader, Effect* effect, InternalEffectVarblock* varblock )
{
	InternalEffectVariable* internalVar = 0;

	// we should create our implementation back-end first
	if (effect->GetType() == Effect::GLSL)
	{
		if (effect->GetMajor() == 4) internalVar = new GLSL4EffectVariable;
	}
	EffectVariable* var = new EffectVariable;

	std::string name = reader->ReadString();
	VariableType type = (VariableType)reader->ReadInt();
	internalVar->type = type;

	// if this is a compute shader variable, read format and access modes
	if (type >= Image1D && type <= ImageCubeArray)
	{
		internalVar->format = (InternalEffectVariable::ImageFormat)reader->ReadInt();
		internalVar->access = (InternalEffectVariable::AccessMode)reader->ReadInt();
	}

    // load annotations
	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		EffectAnnotationStreamLoader loader;
		loader.Load(reader, var);
	}

    // load array if variable is one
	bool isArray = reader->ReadBool();
	if (isArray)
	{
		internalVar->isArray = true;
		internalVar->arraySize = reader->ReadInt();
	}

    // get if variable is subroutine
    bool isSubroutine = reader->ReadBool();
    internalVar->isSubroutine = isSubroutine;

	std::string defaultValue;
	bool hasDefaultValue = reader->ReadBool();
	if (hasDefaultValue)
	{
		defaultValue = reader->ReadString();
		internalVar->hasDefaultValue = true;
	}	

	size_t numPrograms = effect->GetNumPrograms();
	EffectProgram** programs = effect->GetPrograms();
	std::vector<InternalEffectProgram*> internalPrograms;
	internalPrograms.reserve(numPrograms);

	unsigned i;
	for (i = 0; i < numPrograms; i++)
	{
		internalPrograms.push_back(programs[i]->internalProgram);
	}

	internalVar->name = name;
	if (varblock)
	{
		internalVar->isInVarblock = true;
		internalVar->parentBlock = varblock;
	}
	
	internalVar->Setup(internalPrograms, defaultValue);

	var->internalVariable = internalVar;
	return var;
}
} // namespace AnyFX