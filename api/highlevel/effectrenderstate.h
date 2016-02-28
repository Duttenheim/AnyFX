#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectRenderState
    
    An EffectRenderState is an interface used to set the current drawing state before rendering
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------

#ifdef AFX_API
#error "afxapi.h included before effectrenderstate.h"
#endif

#include "EASTL/string.h"
#include "annotable.h"
#include "enums.h"
namespace AnyFX
{
class InternalEffectRenderState;
class EffectRenderState : public Annotable
{
public:


	/// constructor
	EffectRenderState();
	/// destructor
	virtual ~EffectRenderState();

	/// discard program
	void Discard();
    /// resets render state to the default one
    void Reset();

	/// applies render state
	void Apply();
	/// gets name of render state
    const eastl::string& GetName() const;

	/// overrides blend mode flag
	void SetBlendModeFlag(unsigned renderTarget, BlendModeFlag flag, BlendMode mode);
	/// overrides blend function flag
	void SetBlendFuncFlag(unsigned renderTarget, BlendFuncFlag flag, BlendOperation op);
	/// overrides blend bool flag
	void SetBlendBoolFlag(unsigned renderTarget, BlendBoolFlag flag, bool value);
	/// overrides stencil op flag
	void SetStencilOpFlag(StencilOpFlag flag, StencilOperation op);
	/// overrides stencil function flag
	void SetStencilFuncFlag(StencilFuncFlag flag, ComparisonFunc func);
	/// overrides depth function
	void SetDepthFunc(ComparisonFunc func);
	/// overrides fill mode
	void SetFillMode(DrawFillMode fillMode);
	/// overrides cull mode
	void SetCullMode(DrawCullMode cullMode);
	/// sets stencil read and write respectively
	void SetStencilReadWrite(unsigned read, unsigned write);
private:
	friend class EffectRenderStateStreamLoader;
	friend class EffectProgramStreamLoader;

	InternalEffectRenderState* internalRenderState;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------