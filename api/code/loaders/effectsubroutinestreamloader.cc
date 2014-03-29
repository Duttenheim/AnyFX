//------------------------------------------------------------------------------
//  effectsubroutinestreamloader.cc
//  (C) 2014 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "effectsubroutinestreamloader.h"
#include "internal/internaleffectsubroutine.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectSubroutineStreamLoader::EffectSubroutineStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectSubroutineStreamLoader::~EffectSubroutineStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectSubroutine*
EffectSubroutineStreamLoader::Load( BinReader* reader, Effect* effect )
{
    InternalEffectSubroutine* internalSubroutine = new InternalEffectSubroutine;

    // read data from string
    std::string name = reader->ReadString();
    unsigned type = reader->ReadInt();

    // setup internal object
    internalSubroutine->name = name;
    internalSubroutine->type = (EffectSubroutine::SubroutineType)type;

    // create subroutine and return
    EffectSubroutine* subroutine = new EffectSubroutine;
    subroutine->internalSubroutine = internalSubroutine;
    return subroutine;
}
} // namespace AnyFX