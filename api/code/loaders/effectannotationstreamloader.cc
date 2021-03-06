//------------------------------------------------------------------------------
//  effectannotationstreamloader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectannotationstreamloader.h"
#include "annotable.h"
#include "EASTL/string.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectAnnotationStreamLoader::EffectAnnotationStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectAnnotationStreamLoader::~EffectAnnotationStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
EffectAnnotationStreamLoader::Load(BinReader* reader, Annotable* object)
{
	int numAnnotations = reader->ReadInt();
	int i;
	for (i = 0; i < numAnnotations; i++)
	{
		// read annotation name
        eastl::string name = reader->ReadString().c_str();

		// read type
		VariableType type = (VariableType)reader->ReadInt();

		// read value based on type
		Annotable::AnnotationVariant value;

		switch (type)
		{
		case Bool:
			value.data.boolValue = reader->ReadBool();
			break;
		case Integer:
			value.data.intValue = reader->ReadInt();
			break;
		case Double:
			value.data.doubleValue = reader->ReadDouble();
			break;
		case Float:
			value.data.floatValue = reader->ReadFloat();
			break;
		case String:
			{
                value.data.stringValue = new eastl::string;
				*value.data.stringValue = reader->ReadString().c_str();
			}			
			break;
		}

		// set type in variant structure
		value.type = type;

		// add value in object
		object->annotationMap[name] = value;
	}
}
} // namespace AnyFX