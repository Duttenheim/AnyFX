#pragma once
//------------------------------------------------------------------------------
/**
	Contains the most basic representation of a render state.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
#include "enums.h"
namespace AnyFX
{
struct RenderStateBase : public Annotable
{
public:
	/// constructor
	RenderStateBase();
	/// destructor
	virtual ~RenderStateBase();

	static const unsigned MaxNumRenderTargets = 8;

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

	std::string name;

protected:
	friend class RenderStateLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX