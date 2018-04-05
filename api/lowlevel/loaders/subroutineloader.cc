//------------------------------------------------------------------------------
//  subroutineloader.cc
//  (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "subroutineloader.h"
#include <string>
#include "base/subroutinebase.h"
#include "shadereffect.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
SubroutineLoader::SubroutineLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
SubroutineLoader::~SubroutineLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
SubroutineBase*
SubroutineLoader::Load(BinReader* reader, ShaderEffect* effect)
{
	SubroutineBase* subroutine = new SubroutineBase;

    // read data from string
    std::string name = reader->ReadString().c_str();
    unsigned type = reader->ReadInt();

    // setup internal object
	subroutine->name = name;
	subroutine->type = type;

	return subroutine;
}
} // namespace AnyFX