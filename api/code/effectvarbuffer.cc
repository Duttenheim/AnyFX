//------------------------------------------------------------------------------
//  effectvarbuffer.cc
//  (C) 2014 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectvarbuffer.h"
#include "internal/internaleffectvarbuffer.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectVarbuffer::EffectVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVarbuffer::~EffectVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectVarbuffer::Discard()
{
    delete this->internalVarbuffer;
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectVarbuffer::Commit()
{

}

//------------------------------------------------------------------------------
/**
*/
const std::string& 
EffectVarbuffer::GetName() const
{
    return this->internalVarbuffer->GetName();
}
} // namespace AnyFX