#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectVarbuffer
    
    A variable buffer is a buffer of variables which can be variable in size.
    A varbuffer can also be written/read to from a shader.
    
    (C) 2014 Gustav Sterbrant
*/
//------------------------------------------------------------------------------

#ifdef AFX_API
#error "afxapi.h included before effectvarbuffer.h"
#endif

#include <string>
namespace AnyFX
{
class InternalEffectVarbuffer;
class EffectVarbuffer
{
public:
	/// constructor
	EffectVarbuffer();
	/// destructor
	virtual ~EffectVarbuffer();

    /// discard varblock
    void Discard();

    /// commits varblock
    void Commit();
    /// returns name of varblock
    const std::string& GetName() const;

    /// 

private:
    friend class EffectVarbufferStreamLoader;

    InternalEffectVarbuffer* internalVarbuffer;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------