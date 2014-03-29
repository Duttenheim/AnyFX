//------------------------------------------------------------------------------
//  internaleffectvarbuffer.cc
//  (C) 2014 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "internaleffectvarbuffer.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
InternalEffectVarbuffer::InternalEffectVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
InternalEffectVarbuffer::~InternalEffectVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
    Override in subclass!
*/
int 
InternalEffectVarbuffer::GetHandle() const
{
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarbuffer::Apply()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarbuffer::Commit()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarbuffer::SetVariable( InternalEffectVariable* var, void* value, int index )
{

}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarbuffer::SetVariableArray( InternalEffectVariable* var, void* value, size_t size, int index )
{

}

//------------------------------------------------------------------------------
/**
*/
void 
InternalEffectVarbuffer::Activate( InternalEffectProgram* program )
{

}
} // namespace AnyFX