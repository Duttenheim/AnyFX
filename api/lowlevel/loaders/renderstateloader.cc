//------------------------------------------------------------------------------
//  renderstateloader.cc
//  (C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "renderstateloader.h"
#include "shadereffect.h"
#include "base/renderstatebase.h"
#include "annotationloader.h"
#include <string.h>

#include "gl4/gl4renderstate.h"
#include "vk/vkrenderstate.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
RenderStateLoader::RenderStateLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
RenderStateLoader::~RenderStateLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
RenderStateBase* 
RenderStateLoader::Load(BinReader* reader, ShaderEffect* effect)
{
	RenderStateBase* renderState = 0;

	// we should create our implementation back-end first
	if (effect->header == Implementation::GLSL)
	{
		if (effect->major == 4) renderState = new GL4RenderState;
	}
	else if (effect->header == Implementation::SPIRV)
	{
		renderState = new VkRenderState;
	}
	else
	{
		renderState = new RenderStateBase;
	}

	// get name
    std::string name = reader->ReadString().c_str();
	renderState->name = name;

	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		AnnotationLoader loader;
		loader.Load(reader, renderState);
	}

	unsigned i;
	for (i = 0; i < RenderStateBase::MaxNumRenderTargets; i++)
	{
		bool blendEnabled = reader->ReadBool();
		int sourceBlend = reader->ReadInt();
		int destinationBlend = reader->ReadInt();
		int blendOp = reader->ReadInt();
		int sourceBlendAlpha = reader->ReadInt();
		int destinationBlendAlpha = reader->ReadInt();
		int blendOpAlpha = reader->ReadInt();

		renderState->renderSettings.blendEnabled[i]		= blendEnabled;
		renderState->renderSettings.srcBlends[i]		= (BlendMode)sourceBlend;
		renderState->renderSettings.dstBlends[i]		= (BlendMode)destinationBlend;
		renderState->renderSettings.blendOps[i]			= (BlendOperation)blendOp;
		renderState->renderSettings.alphaSrcBlends[i]	= (BlendMode)sourceBlendAlpha;
		renderState->renderSettings.alphaDstBlends[i]	= (BlendMode)destinationBlendAlpha;
		renderState->renderSettings.alphaBlendOps[i]	= (BlendOperation)blendOpAlpha;
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
	bool polygonOffsetEnabled = reader->ReadBool();
	bool rasterizerDiscardEnabled = reader->ReadBool();
	bool logicOpEnabled = reader->ReadBool();

	renderState->renderSettings.depthEnabled				= depthEnabled;
	renderState->renderSettings.depthWriteEnabled			= depthWrite;
	renderState->renderSettings.depthClampEnabled			= depthClamp;
	renderState->renderSettings.separateBlendEnabled		= separateBlend;
	renderState->renderSettings.scissorEnabled				= scissorEnabled;
	renderState->renderSettings.stencilEnabled				= stencilEnabled;
	renderState->renderSettings.alphaToCoverageEnabled		= alphaToCoverageEnabled;
	renderState->renderSettings.alphaToOneEnabled			= alphaToOneEnabled;
	renderState->renderSettings.multisampleEnabled			= multisampleEnabled;
	renderState->renderSettings.rasterizerDiscardEnabled	= rasterizerDiscardEnabled;
	renderState->renderSettings.logicOpEnabled				= logicOpEnabled;
	renderState->renderSettings.polygonOffsetEnabled		= polygonOffsetEnabled;

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

	renderState->renderSettings.depthFunction		= (ComparisonFunc)depthFunc;
	renderState->renderSettings.cullMode			= (DrawCullMode)cullMode;
	renderState->renderSettings.fillMode			= (DrawFillMode)rasterizerMode;
	renderState->renderSettings.windingMode			= (DrawWindingMode)windingMode;
	renderState->renderSettings.sampleCount			= (DrawSampleCount)sampleCount;
	renderState->renderSettings.logicOp				= (DrawLogicalOperation)logicOp;
	renderState->renderSettings.frontFailOp			= (StencilOperation)stencilFrontFail;
	renderState->renderSettings.backFailOp			= (StencilOperation)stencilBackFail;
	renderState->renderSettings.frontPassOp			= (StencilOperation)stencilFrontPass;
	renderState->renderSettings.backPassOp			= (StencilOperation)stencilBackPass;
	renderState->renderSettings.frontDepthFailOp	= (StencilOperation)stencilFrontDepthFail;
	renderState->renderSettings.backDepthFailOp		= (StencilOperation)stencilBackDepthFail;
	renderState->renderSettings.frontFaceFunc		= (ComparisonFunc)stencilFrontFunc;
	renderState->renderSettings.backFaceFunc		= (ComparisonFunc)stencilBackFunc;

	int stencilFrontRef = reader->ReadInt();
	int stencilBackRef = reader->ReadInt();
	unsigned stencilReadMask = reader->ReadUInt();
	unsigned stencilWriteMask = reader->ReadUInt();

	renderState->renderSettings.frontRef			= stencilFrontRef;
	renderState->renderSettings.backRef				= stencilBackRef;
	renderState->renderSettings.stencilReadMask		= stencilReadMask;
	renderState->renderSettings.stencilWriteMask	= stencilWriteMask;

	float polygonOffsetFactor = reader->ReadFloat();
	float polygonOffsetUnits = reader->ReadFloat();
	float minSampleShading = reader->ReadFloat();
	float minDepthBounds = reader->ReadFloat();
	float maxDepthBounds = reader->ReadFloat();
	float lineWidth = reader->ReadFloat();

	renderState->renderSettings.polygonOffsetFactor = polygonOffsetFactor;
	renderState->renderSettings.polygonOffsetUnits = polygonOffsetUnits;
	renderState->renderSettings.minSampleShading = minSampleShading;
	renderState->renderSettings.minDepthBounds = minDepthBounds;
	renderState->renderSettings.maxDepthBounds = maxDepthBounds;
	renderState->renderSettings.lineWidth = lineWidth;

    // memcpy default values loaded from file to interface
	memcpy(&renderState->defaultRenderSettings, &renderState->renderSettings, sizeof(RenderStateBase::RenderStateSettings));

	renderState->OnLoaded();
	return renderState;
}
} // namespace AnyFX
