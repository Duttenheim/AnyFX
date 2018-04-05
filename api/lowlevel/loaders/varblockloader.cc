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
#include "types.h"

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
VarblockLoader::Load(BinReader* reader, ShaderEffect* effect, std::vector<VariableBase*>& vars)
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

	std::string name = reader->ReadString().c_str();
	unsigned alignedSize = reader->ReadUInt();
	Qualifiers qualifierFlags = FromInteger(reader->ReadUInt());
	
	unsigned binding = reader->ReadUInt();
	unsigned set = reader->ReadUInt();	

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

	// load offsets
	unsigned numOffsets = reader->ReadUInt();
	for (i = 0; i < numOffsets; i++)
	{
		std::string name = reader->ReadString().c_str();
		unsigned offset = reader->ReadUInt();
		varblock->offsetsByName[name] = offset;
	}

    // set internal variables
	varblock->name = name;
	varblock->alignedSize = alignedSize;
	varblock->qualifiers = qualifierFlags;
	varblock->set = set;
	varblock->binding = binding;

	varblock->OnLoaded();
	return varblock;
}
} // namespace AnyFX
