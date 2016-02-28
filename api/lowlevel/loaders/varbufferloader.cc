//------------------------------------------------------------------------------
//  varbufferloader.cc
//  (C) 2014 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "varbufferloader.h"
#include "shadereffect.h"
#include "base/varbufferbase.h"
#include "gl4/gl4varbuffer.h"
#include "vk/vkvarbuffer.h"
#include "annotationloader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VarbufferLoader::VarbufferLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VarbufferLoader::~VarbufferLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VarbufferBase*
VarbufferLoader::Load(BinReader* reader, ShaderEffect* effect)
{
	VarbufferBase* varbuffer = 0;

	if (effect->header == Implementation::GLSL)
    {
		if (effect->major == 4) varbuffer = new GL4Varbuffer;
    }
	else if (effect->header == Implementation::SPIRV)
	{
		varbuffer = new VkVarbuffer;
	}
	else
	{
		varbuffer = new VarbufferBase;
	}

	// start loading
	eastl::string name = reader->ReadString().c_str();
	bool shared = reader->ReadBool();
	unsigned binding = reader->ReadUInt();
	unsigned set = reader->ReadUInt();

	// load annotations
	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		AnnotationLoader loader;
		loader.Load(reader, varbuffer);
	}

	// load size of buffer
	unsigned size = reader->ReadUInt();

	// load internal buffer
	varbuffer->name = name;
	varbuffer->size = size;
	varbuffer->isShared = shared;
	varbuffer->set = set;
	varbuffer->binding = binding;

	varbuffer->OnLoaded();
    return varbuffer;
}
} // namespace AnyFX