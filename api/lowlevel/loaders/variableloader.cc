//------------------------------------------------------------------------------
//  variableloader.cc
//  (C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "variableloader.h"
#include "base/variablebase.h"
#include "base/programbase.h"
#include "shadereffect.h"
#include "annotationloader.h"
#include "vk/vkvariable.h"
#include "gl4/gl4variable.h"
#include "base/varblockbase.h"

namespace AnyFX
{
//------------------------------------------------------------------------------
/**
*/
VariableLoader::VariableLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VariableLoader::~VariableLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VariableBase*
VariableLoader::Load(BinReader* reader, ShaderEffect* effect, VarblockBase* varblock)
{
	VariableBase* var = 0;

	// we should create our implementation back-end first
	if (effect->header == Implementation::GLSL)
	{
		if (effect->major == 4) var = new GL4Variable;
	}
	else if (effect->header == Implementation::SPIRV)
	{
		var = new VkVariable;
	}
	else
	{
		var = new VariableBase;
	}

    std::string name = reader->ReadString().c_str();
    bool shared = reader->ReadBool();       
    bool bindless = reader->ReadBool();
	unsigned binding = reader->ReadUInt();
	unsigned set = reader->ReadUInt();
    int type = reader->ReadInt();

	var->type = (VariableType)type;
    var->bindless = bindless;
	var->set = set;
	var->binding = binding;

	// if this is a compute shader variable, read format and access modes
	if (type >= Image1D && type <= ImageCubeArray)
	{
		var->format = reader->ReadInt();
		var->access = reader->ReadInt();
	}

    // load annotations
	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		AnnotationLoader loader;
		loader.Load(reader, var);
	}

    // load array if variable is one
	bool isArray = reader->ReadBool();
	if (isArray)
	{
		var->isArray = true;
		var->arraySize = reader->ReadInt();
	}

    // get if variable is subroutine
    bool isSubroutine = reader->ReadBool();
	var->isSubroutine = isSubroutine;

	std::string defaultValue;
	bool hasDefaultValue = reader->ReadBool();
	if (hasDefaultValue)
	{
		defaultValue = reader->ReadString().c_str();
		var->hasDefaultValue = true;
		var->defaultValueString = defaultValue;
	}	

	var->name = name;
	if (varblock)
	{
		var->isInVarblock = true;
		var->parentBlock = varblock;
	}

	var->OnLoaded();
	return var;
}
} // namespace AnyFX