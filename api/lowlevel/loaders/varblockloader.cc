//------------------------------------------------------------------------------
//  varblockloader.cc
//  (C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "varblockloader.h"
#include "shadereffect.h"
#include "base/varblockbase.h"
#include "gl4/gl4varblock.h"
#include "vk/vkvarblock.h"
#include "annotationloader.h"
#include "variableloader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VarblockLoader::VarblockLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VarblockLoader::~VarblockLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VarblockBase* 
VarblockLoader::Load(BinReader* reader, ShaderEffect* effect, eastl::vector<VariableBase*>& vars)
{
	VarblockBase* varblock = 0;

	// we should create our implementation back-end first
	if (effect->header == Implementation::GLSL)
	{
		if (effect->major == 4) varblock = new GL4Varblock;
	}
	else if (effect->header == Implementation::SPIRV)
	{
		varblock = new VkVarblock;
	}
	else
	{
		varblock = new VarblockBase;
	}

	eastl::string name = reader->ReadString().c_str();
	bool shared = reader->ReadBool();
	unsigned binding = reader->ReadUInt();
	unsigned set = reader->ReadUInt();
	bool push = reader->ReadBool();

    // load annotations
	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		AnnotationLoader loader;
		loader.Load(reader, varblock);
	}

    // load variables
	unsigned numVars = reader->ReadInt();
	VariableLoader variableLoader;
	unsigned i;
	for (i = 0; i < numVars; i++)
	{
		// read variable
		VariableBase* var = variableLoader.Load(reader, effect, varblock);
		varblock->variables.push_back(var);
		varblock->variablesByName[var->name] = var;
		
		// make sure to flag this variable as internal to the varblock
		vars.push_back(var);
	}

    // set internal variables
	varblock->name = name;
	varblock->isShared = shared;
	varblock->set = set;
	varblock->binding = binding;
	varblock->push = push;
	varblock->SetupSignature();

	varblock->OnLoaded();
	return varblock;
}
} // namespace AnyFX
