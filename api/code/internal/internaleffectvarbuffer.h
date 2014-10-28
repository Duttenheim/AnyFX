#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::InternalEffectVarbuffer
    
    Internal representation of variable buffer. Base class for all implementations of variable buffers.
    
    (C) 2014 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "EASTL/vector.h"
#include <string>
namespace AnyFX
{
class InternalEffectVariable;
class InternalEffectProgram;
class InternalEffectVarbuffer
{
public:
	/// constructor
	InternalEffectVarbuffer();
	/// destructor
	virtual ~InternalEffectVarbuffer();

    /// returns the handle to the internal object, in OpenGL this is an OpenGL name, in DirectX this is a pointer to an object.
    virtual int GetHandle() const;
    
    /// returns name of varbuffer
    const std::string& GetName() const;

protected:
    friend class EffectVarbufferStreamLoader;

    /// applies varbuffer into the current context
    virtual void Apply();
    /// commits changes made to the varbuffer
    virtual void Commit();

    /// updates single variable
    void SetVariable(InternalEffectVariable* var, void* value, int index);
    /// updates variable array
    void SetVariableArray(InternalEffectVariable* var, void* value, size_t size, int index);
    /// activates variable, this makes the uniform location be the one found in the given program
    virtual void Activate(InternalEffectProgram* program);

    eastl::vector<InternalEffectVariable*> variables;
    InternalEffectVarbuffer* masterBuffer;
    std::string name;
}; 


//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
InternalEffectVarbuffer::GetName() const
{
    return this->name;
}


} // namespace AnyFX
//------------------------------------------------------------------------------