//------------------------------------------------------------------------------
//  effectvarbufferstreamloader.cc
//  (C) 2014 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectvarbufferstreamloader.h"
#include "effect.h"
#include "effectvarbuffer.h"
#include "internal/internaleffectvarbuffer.h"
#include "internal/glsl4/glsl4effectvarbuffer.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectVarbufferStreamLoader::EffectVarbufferStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVarbufferStreamLoader::~EffectVarbufferStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVarbuffer*
EffectVarbufferStreamLoader::Load( BinReader* reader, Effect* effect )
{
    InternalEffectVarbuffer* internalVarbuffer = 0;

    if (effect->GetType() == Effect::GLSL)
    {
        if (effect->GetMajor() == 4) internalVarbuffer = new GLSL4EffectVarbuffer;
    }

    EffectVarbuffer* varbuffer = new EffectVarbuffer;
    varbuffer->internalVarbuffer = internalVarbuffer;
    return varbuffer;
}
} // namespace AnyFX