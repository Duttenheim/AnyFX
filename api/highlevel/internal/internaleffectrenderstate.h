#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::InternalEffectRenderState
    
    EffectRenderState backend, inherit this class to provide an implementation
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include "effectrenderstate.h"
#include "enums.h"
namespace AnyFX
{
class Effect;
class InternalEffectRenderState
{
public:
	/// constructor
	InternalEffectRenderState();
	/// destructor
	virtual ~InternalEffectRenderState();

	static const unsigned MaxNumRenderTargets = 8;

protected:
	friend class EffectRenderState;
	friend class EffectRenderStateStreamLoader;
	friend class InternalEffectProgram;

	/// applies render state, override in implementation
	virtual void Apply();
    /// resets render state
    void Reset();

	/// gets name of render state
    const eastl::string& GetName() const;
	
	Effect* effect;

	struct RenderStateSettings
	{
		bool blendEnabled[MaxNumRenderTargets];
		BlendMode srcBlends[MaxNumRenderTargets];
		BlendMode dstBlends[MaxNumRenderTargets];
		BlendOperation blendOps[MaxNumRenderTargets];
		BlendMode alphaSrcBlends[MaxNumRenderTargets];
		BlendMode alphaDstBlends[MaxNumRenderTargets];
		BlendOperation alphaBlendOps[MaxNumRenderTargets];

		StencilOperation frontFailOp;
		StencilOperation backFailOp;
		StencilOperation frontPassOp;
		StencilOperation backPassOp;
		StencilOperation frontDepthFailOp;
		StencilOperation backDepthFailOp;
		ComparisonFunc frontFaceFunc;
		ComparisonFunc backFaceFunc;
		int frontRef;
		int backRef;

		unsigned stencilReadMask;
		unsigned stencilWriteMask;
		ComparisonFunc depthFunction;
		DrawCullMode cullMode;
		DrawFillMode fillMode;
		DrawWindingMode windingMode;
		DrawSampleCount sampleCount;
		DrawLogicalOperation logicOp;

		bool depthEnabled;
		bool depthWriteEnabled;
		bool depthClampEnabled;
		bool separateBlendEnabled;
		bool scissorEnabled;
		bool stencilEnabled;
		bool alphaToCoverageEnabled;
		bool alphaToOneEnabled;
		bool multisampleEnabled;
		bool rasterizerDiscardEnabled;
		bool logicOpEnabled;
		bool polygonOffsetEnabled;
		float polygonOffsetFactor;
		float polygonOffsetUnits;
		float minSampleShading;
		float minDepthBounds;
		float maxDepthBounds;
		float lineWidth;
	} renderSettings, defaultRenderSettings;

    eastl::string name;
}; 


//------------------------------------------------------------------------------
/**
*/
inline const eastl::string&
InternalEffectRenderState::GetName() const
{
	return this->name;
}

} // namespace AnyFX
//------------------------------------------------------------------------------