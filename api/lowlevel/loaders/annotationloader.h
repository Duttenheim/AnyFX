#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectAnnotationStreamLoader
    
    Loads annotations attached to effect objects.
    
    (C) 2016 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binreader.h"
#include "annotable.h"
namespace AnyFX
{
class Annotable;
class AnnotationLoader
{
public:
	/// constructor
	AnnotationLoader();
	/// destructor
	virtual ~AnnotationLoader();

private:
	friend class StreamLoader;
	friend class RenderStateLoader;
	friend class VarblockLoader;
	friend class VariableLoader;
	friend class VarbufferLoader;
	friend class ProgramLoader;

	// load annotation and save into object
	void Load(BinReader* reader, Annotable* object);
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------