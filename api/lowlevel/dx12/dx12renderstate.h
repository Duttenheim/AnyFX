#pragma once
//------------------------------------------------------------------------------
/**
	Contains the most basic representation of a render state.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------

namespace AnyFX
{
struct DX12RenderState
{
public:
	/// constructor
	RenderStateBase();
	/// destructor
	virtual ~RenderStateBase();
private:
};
} // namespace AnyFX