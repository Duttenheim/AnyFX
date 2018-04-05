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
#include "types.h"

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
	std::string name = reader->ReadString().c_str();
	unsigned alignedSize = reader->ReadUInt();
	unsigned size = reader->ReadUInt();
	Qualifiers qualifierFlags = FromInteger(reader->ReadUInt());
	unsigned binding = reader->ReadUInt();
	unsigned set = reader->ReadUInt();

	// load annotations
	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		AnnotationLoader loader;
		loader.Load(reader, varbuffer);
	}

	// load offsets
	unsigned numOffsets = reader->ReadUInt();
	unsigned i;
	for (i = 0; i < numOffsets; i++)
	{
		std::string name = reader->ReadString().c_str();
		unsigned offset = reader->ReadUInt();
		varbuffer->offsetsByName[name] = offset;
	}

	// load internal buffer
	varbuffer->name = name;
	varbuffer->alignedSize = alignedSize;
	varbuffer->size = size;
	varbuffer->qualifiers = qualifierFlags;
	varbuffer->set = set;
	varbuffer->binding = binding;

	varbuffer->OnLoaded();
    return varbuffer;
}
} // namespace AnyFX