//------------------------------------------------------------------------------
// vkrenderstate.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkrenderstate.h"

namespace AnyFX
{

/// define a conversion table to map blend modes from AnyFX to OpenGL
static const VkBlendFactor vkBlendTable[] =
{
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_SRC_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	VK_BLEND_FACTOR_DST_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	VK_BLEND_FACTOR_SRC_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_FACTOR_DST_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	VK_BLEND_FACTOR_CONSTANT_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
	VK_BLEND_FACTOR_CONSTANT_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
	VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
	VK_BLEND_FACTOR_SRC1_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
	VK_BLEND_FACTOR_SRC1_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
};

static const VkBlendOp vkEquationTable[] =
{
	VK_BLEND_OP_ADD,
	VK_BLEND_OP_SUBTRACT,
	VK_BLEND_OP_REVERSE_SUBTRACT,
	VK_BLEND_OP_MIN,
	VK_BLEND_OP_MAX
};

static const VkLogicOp vkLogicTable[] =
{
	VK_LOGIC_OP_CLEAR,
	VK_LOGIC_OP_AND,
	VK_LOGIC_OP_AND_REVERSE,
	VK_LOGIC_OP_COPY,
	VK_LOGIC_OP_AND_INVERTED,
	VK_LOGIC_OP_NO_OP,
	VK_LOGIC_OP_XOR,
	VK_LOGIC_OP_OR,
	VK_LOGIC_OP_NOR,
	VK_LOGIC_OP_EQUIVALENT,
	VK_LOGIC_OP_INVERT,
	VK_LOGIC_OP_OR_REVERSE,
	VK_LOGIC_OP_COPY_INVERTED,
	VK_LOGIC_OP_OR_INVERTED,
	VK_LOGIC_OP_NAND,
	VK_LOGIC_OP_SET,
};

static const VkCompareOp vkComparisonTable[] =
{
	VK_COMPARE_OP_NEVER,
	VK_COMPARE_OP_LESS,
	VK_COMPARE_OP_LESS_OR_EQUAL,
	VK_COMPARE_OP_GREATER,
	VK_COMPARE_OP_GREATER_OR_EQUAL,
	VK_COMPARE_OP_EQUAL,
	VK_COMPARE_OP_NOT_EQUAL,
	VK_COMPARE_OP_ALWAYS
};

static const VkStencilOp vkStencilOpTable[] =
{
	VK_STENCIL_OP_KEEP,
	VK_STENCIL_OP_ZERO,
	VK_STENCIL_OP_REPLACE,
	VK_STENCIL_OP_INCREMENT_AND_CLAMP,
	VK_STENCIL_OP_INCREMENT_AND_WRAP,
	VK_STENCIL_OP_DECREMENT_AND_CLAMP,
	VK_STENCIL_OP_DECREMENT_AND_WRAP,
	VK_STENCIL_OP_INVERT
};

static const VkCullModeFlags vkCullTable[] =
{
	VK_CULL_MODE_BACK_BIT,
	VK_CULL_MODE_FRONT_BIT,
	VK_CULL_MODE_NONE
};

// in reality, this order should be reversed, but this is the GL convention
static const VkFrontFace vkFaceTable[] =
{
	VK_FRONT_FACE_COUNTER_CLOCKWISE,		
	VK_FRONT_FACE_CLOCKWISE	
};

static const VkPolygonMode vkPolygonModeTable[] =
{
	VK_POLYGON_MODE_FILL,
	VK_POLYGON_MODE_LINE,
	VK_POLYGON_MODE_POINT
};

static const VkSampleCountFlagBits vkSampleCountTable[] =
{
	VK_SAMPLE_COUNT_1_BIT,
	VK_SAMPLE_COUNT_2_BIT,
	VK_SAMPLE_COUNT_4_BIT,
	VK_SAMPLE_COUNT_8_BIT,
	VK_SAMPLE_COUNT_16_BIT,
	VK_SAMPLE_COUNT_32_BIT,
	VK_SAMPLE_COUNT_64_BIT,
};

//------------------------------------------------------------------------------
/**
*/
VkRenderState::VkRenderState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkRenderState::~VkRenderState()
{
	// empty
}


//------------------------------------------------------------------------------
/**
*/
void
VkRenderState::SetupRasterization(VkPipelineRasterizationStateCreateInfo* info)
{
	info->depthClampEnable = this->renderSettings.depthClampEnabled;
	info->rasterizerDiscardEnable = this->renderSettings.rasterizerDiscardEnabled;
	info->polygonMode = vkPolygonModeTable[this->renderSettings.fillMode];
	info->cullMode = vkCullTable[this->renderSettings.cullMode];
	info->frontFace = vkFaceTable[this->renderSettings.windingMode];
	info->depthBiasEnable = this->renderSettings.polygonOffsetEnabled;
	info->depthBiasConstantFactor = this->renderSettings.polygonOffsetUnits;
	info->depthBiasSlopeFactor = this->renderSettings.polygonOffsetFactor;
	info->lineWidth = this->renderSettings.lineWidth;
}

//------------------------------------------------------------------------------
/**
*/
void
VkRenderState::SetupMultisample(VkPipelineMultisampleStateCreateInfo* info)
{
	info->rasterizationSamples = vkSampleCountTable[this->renderSettings.sampleCount];
	info->sampleShadingEnable = this->renderSettings.multisampleEnabled;
	info->minSampleShading = this->renderSettings.minSampleShading;
	info->pSampleMask = NULL;		// FIXME
	info->alphaToCoverageEnable = this->renderSettings.alphaToCoverageEnabled;
	info->alphaToOneEnable = this->renderSettings.alphaToOneEnabled;
}


//------------------------------------------------------------------------------
/**
*/
void
VkRenderState::SetupDepthStencil(VkPipelineDepthStencilStateCreateInfo* info)
{
	info->depthTestEnable = this->renderSettings.depthEnabled;
	info->depthWriteEnable = this->renderSettings.depthWriteEnabled;
	info->depthCompareOp = vkComparisonTable[this->renderSettings.depthFunction];
	info->depthBoundsTestEnable = this->renderSettings.depthClampEnabled;
	info->stencilTestEnable = this->renderSettings.stencilEnabled;

	VkStencilOpState frontFace, backFace;
	frontFace.failOp = vkStencilOpTable[this->renderSettings.frontFailOp];
	frontFace.passOp = vkStencilOpTable[this->renderSettings.frontPassOp];
	frontFace.depthFailOp = vkStencilOpTable[this->renderSettings.frontDepthFailOp];
	frontFace.compareOp = vkComparisonTable[this->renderSettings.frontFaceFunc];
	frontFace.compareMask = this->renderSettings.stencilReadMask;
	frontFace.writeMask = this->renderSettings.stencilWriteMask;
	frontFace.reference = this->renderSettings.frontRef;

	backFace.failOp = vkStencilOpTable[this->renderSettings.backFailOp];
	backFace.passOp = vkStencilOpTable[this->renderSettings.backPassOp];
	backFace.depthFailOp = vkStencilOpTable[this->renderSettings.backDepthFailOp];
	backFace.compareOp = vkComparisonTable[this->renderSettings.backFaceFunc];
	backFace.compareMask = this->renderSettings.stencilReadMask;
	backFace.writeMask = this->renderSettings.stencilWriteMask;
	backFace.reference = this->renderSettings.backRef;

	info->front = frontFace;
	info->back = backFace;
	info->minDepthBounds = this->renderSettings.minDepthBounds;
	info->maxDepthBounds = this->renderSettings.maxDepthBounds;
}

//------------------------------------------------------------------------------
/**
*/
void
VkRenderState::SetupBlend(VkPipelineColorBlendStateCreateInfo* info)
{
	info->logicOp = vkLogicTable[this->renderSettings.logicOp];
	info->logicOpEnable = this->renderSettings.logicOpEnabled;

	unsigned i;
	for (i = 0; i < 8; i++)
	{
		this->states[i].blendEnable = this->renderSettings.blendEnabled[i];
		this->states[i].colorBlendOp = vkEquationTable[this->renderSettings.blendOps[i]];
		this->states[i].alphaBlendOp = vkEquationTable[this->renderSettings.alphaBlendOps[i]];
		this->states[i].srcColorBlendFactor = vkBlendTable[this->renderSettings.srcBlends[i]];
		this->states[i].dstColorBlendFactor = vkBlendTable[this->renderSettings.dstBlends[i]];
		this->states[i].srcAlphaBlendFactor = vkBlendTable[this->renderSettings.alphaSrcBlends[i]];
		this->states[i].dstAlphaBlendFactor = vkBlendTable[this->renderSettings.alphaDstBlends[i]];

		// FIXME
		this->states[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	}
	info->attachmentCount = 8;
	info->pAttachments = this->states;
}

} // namespace AnyFX