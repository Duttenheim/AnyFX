//------------------------------------------------------------------------------
//  effectrenderstatestreamloader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectrenderstatestreamloader.h"
#include "effectrenderstate.h"
#include "internal/internaleffectrenderstate.h"
#include "internal/glsl4/glsl4effectrenderstate.h"
#include "highlevel/effect.h"
#include "effectannotationstreamloader.h"
#include <string.h>
#include "internal/vk/vkeffectrenderstate.h"
#include "debug.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectRenderStateStreamLoader::EffectRenderStateStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectRenderStateStreamLoader::~EffectRenderStateStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectRenderState*
EffectRenderStateStreamLoader::Load(BinReader* reader, Effect* effect)
{
	InternalEffectRenderState* internalRenderState = 0;

	// we should create our implementation back-end first
	if (effect->GetType() == Implementation::GLSL)
	{
		if (effect->GetMajor() == 4) internalRenderState = new GLSL4EffectRenderState;
	}
	else
	{
		Error("No implementation for '%d'\n", effect->GetType());
	}
	internalRenderState->effect = effect;
	EffectRenderState* renderState = new EffectRenderState;

	// get name
    eastl::string name = reader->ReadString().c_str();
	internalRenderState->name = name;

	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		EffectAnnotationStreamLoader loader;
		loader.Load(reader, renderState);
	}

	unsigned i;
	for (i = 0; i < InternalEffectRenderState::MaxNumRenderTargets; i++)
	{
		bool blendEnabled = reader->ReadBool();
		int sourceBlend = reader->ReadInt();
		int destinationBlend = reader->ReadInt();
		int blendOp = reader->ReadInt();
		int sourceBlendAlpha = reader->ReadInt();
		int destinationBlendAlpha = reader->ReadInt();
		int blendOpAlpha = reader->ReadInt();

		internalRenderState->renderSettings.blendEnabled[i]		= blendEnabled;
		internalRenderState->renderSettings.srcBlends[i]		= (BlendMode)sourceBlend;
		internalRenderState->renderSettings.dstBlends[i]		= (BlendMode)destinationBlend;
		internalRenderState->renderSettings.blendOps[i]			= (BlendOperation)blendOp;
		internalRenderState->renderSettings.alphaSrcBlends[i]	= (BlendMode)sourceBlendAlpha;
		internalRenderState->renderSettings.alphaDstBlends[i]	= (BlendMode)destinationBlendAlpha;
		internalRenderState->renderSettings.alphaBlendOps[i]	= (BlendOperation)blendOpAlpha;
	}

	bool depthEnabled = reader->ReadBool();
	bool depthWrite = reader->ReadBool();
	bool depthClamp = reader->ReadBool();
	bool separateBlend = reader->ReadBool();
	bool scissorEnabled = reader->ReadBool();
	bool stencilEnabled = reader->ReadBool();
	bool alphaToCoverageEnabled = reader->ReadBool();
	bool alphaToOneEnabled = reader->ReadBool();
	bool multisampleEnabled = reader->ReadBool();
	bool rasterizerDiscardEnabled = reader->ReadBool();
	bool logicOpEnabled = reader->ReadBool();
	bool polygonOffsetEnabled = reader->ReadBool();

	internalRenderState->renderSettings.depthEnabled				= depthEnabled;
	internalRenderState->renderSettings.depthWriteEnabled			= depthWrite;
	internalRenderState->renderSettings.depthClampEnabled			= depthClamp;
	internalRenderState->renderSettings.separateBlendEnabled		= separateBlend;
	internalRenderState->renderSettings.scissorEnabled				= scissorEnabled;
	internalRenderState->renderSettings.stencilEnabled				= stencilEnabled;
	internalRenderState->renderSettings.alphaToCoverageEnabled		= alphaToCoverageEnabled;
	internalRenderState->renderSettings.alphaToOneEnabled			= alphaToOneEnabled;
	internalRenderState->renderSettings.multisampleEnabled			= multisampleEnabled;
	internalRenderState->renderSettings.rasterizerDiscardEnabled	= rasterizerDiscardEnabled;
	internalRenderState->renderSettings.logicOpEnabled				= logicOpEnabled;
	internalRenderState->renderSettings.polygonOffsetEnabled		= polygonOffsetEnabled;

	unsigned depthFunc = reader->ReadUInt();
	unsigned cullMode = reader->ReadUInt();
	unsigned rasterizerMode = reader->ReadUInt();
	unsigned windingMode = reader->ReadUInt();
	unsigned sampleCount = reader->ReadUInt();
	unsigned logicOp = reader->ReadUInt();
	unsigned stencilFrontFail = reader->ReadUInt();
	unsigned stencilBackFail = reader->ReadUInt();
	unsigned stencilFrontPass = reader->ReadUInt();
	unsigned stencilBackPass = reader->ReadUInt();
	unsigned stencilFrontDepthFail = reader->ReadUInt();
	unsigned stencilBackDepthFail = reader->ReadUInt();
	unsigned stencilFrontFunc = reader->ReadUInt();
	unsigned stencilBackFunc = reader->ReadUInt();

	internalRenderState->renderSettings.depthFunction		= (ComparisonFunc)depthFunc;
	internalRenderState->renderSettings.cullMode			= (DrawCullMode)cullMode;
	internalRenderState->renderSettings.fillMode			= (DrawFillMode)rasterizerMode;
	internalRenderState->renderSettings.windingMode			= (DrawWindingMode)windingMode;
	internalRenderState->renderSettings.sampleCount			= (DrawSampleCount)sampleCount;
	internalRenderState->renderSettings.logicOp				= (DrawLogicalOperation)logicOp;
	internalRenderState->renderSettings.frontFailOp			= (StencilOperation)stencilFrontFail;
	internalRenderState->renderSettings.backFailOp			= (StencilOperation)stencilBackFail;
	internalRenderState->renderSettings.frontPassOp			= (StencilOperation)stencilFrontPass;
	internalRenderState->renderSettings.backPassOp			= (StencilOperation)stencilBackPass;
	internalRenderState->renderSettings.frontDepthFailOp	= (StencilOperation)stencilFrontDepthFail;
	internalRenderState->renderSettings.backDepthFailOp		= (StencilOperation)stencilBackDepthFail;
	internalRenderState->renderSettings.frontFaceFunc		= (ComparisonFunc)stencilFrontFunc;
	internalRenderState->renderSettings.backFaceFunc		= (ComparisonFunc)stencilBackFunc;

	int stencilFrontRef = reader->ReadInt();
	int stencilBackRef = reader->ReadInt();
	unsigned stencilReadMask = reader->ReadUInt();
	unsigned stencilWriteMask = reader->ReadUInt();

	internalRenderState->renderSettings.frontRef			= stencilFrontRef;
	internalRenderState->renderSettings.backRef				= stencilBackRef;
	internalRenderState->renderSettings.stencilReadMask		= stencilReadMask;
	internalRenderState->renderSettings.stencilWriteMask	= stencilWriteMask;

	float polygonOffsetFactor = reader->ReadFloat();
	float polygonOffsetUnits = reader->ReadFloat();
	float minSampleShading = reader->ReadFloat();
	float minDepthBounds = reader->ReadFloat();
	float maxDepthBounds = reader->ReadFloat();
	float lineWidth = reader->ReadFloat();

	internalRenderState->renderSettings.polygonOffsetFactor = polygonOffsetFactor;
	internalRenderState->renderSettings.polygonOffsetUnits = polygonOffsetUnits;
	internalRenderState->renderSettings.minSampleShading = minSampleShading;
	internalRenderState->renderSettings.minDepthBounds = minDepthBounds;
	internalRenderState->renderSettings.maxDepthBounds = maxDepthBounds;
	internalRenderState->renderSettings.lineWidth = lineWidth;

    // memcpy default values loaded from file to interface
    memcpy(&internalRenderState->defaultRenderSettings, &internalRenderState->renderSettings, sizeof(InternalEffectRenderState::RenderStateSettings));

	renderState->internalRenderState = internalRenderState;
	return renderState;
}
} // namespace AnyFX
