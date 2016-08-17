//------------------------------------------------------------------------------
// vksampler.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vksampler.h"

namespace AnyFX
{

static const VkSamplerAddressMode vkAddressModeTable[] =
{
	VK_SAMPLER_ADDRESS_MODE_REPEAT,
	VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
	VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
	VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
	VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
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

static const VkSamplerMipmapMode vkMipmapTable[] = 
{
	VK_SAMPLER_MIPMAP_MODE_NEAREST,						// MinMagMipPoint
	VK_SAMPLER_MIPMAP_MODE_LINEAR,						// MinMagMipLinear
	VK_SAMPLER_MIPMAP_MODE_LINEAR,						// MinMagPointMipLinear
	VK_SAMPLER_MIPMAP_MODE_NEAREST,						// MinMipPointMagLinear
	VK_SAMPLER_MIPMAP_MODE_LINEAR,						// MinPointMipMagLinear
	VK_SAMPLER_MIPMAP_MODE_NEAREST,						// MinLinearMipMagPoint
	VK_SAMPLER_MIPMAP_MODE_LINEAR,						// MinMipLinearMagPoint
	VK_SAMPLER_MIPMAP_MODE_NEAREST,						// MinMagLinearMipPoint
	VK_SAMPLER_MIPMAP_MODE_LINEAR,						// Anisotropic
	VK_SAMPLER_MIPMAP_MODE_NEAREST,						// Point
	VK_SAMPLER_MIPMAP_MODE_LINEAR,						// Linear
};

static const VkFilter vkMinTable[] =
{
	VK_FILTER_NEAREST,									// MinMagMipPoint
	VK_FILTER_LINEAR,									// MinMagMipLinear
	VK_FILTER_NEAREST,									// MinMagPointMipLinear
	VK_FILTER_NEAREST,									// MinMipPointMagLinear
	VK_FILTER_NEAREST,									// MinPointMipMagLinear
	VK_FILTER_LINEAR,									// MinLinearMipMagPoint
	VK_FILTER_LINEAR,									// MinMipLinearMagPoint
	VK_FILTER_LINEAR,									// MinMagLinearMipPoint
	VK_FILTER_LINEAR,									// Anisotropic
	VK_FILTER_NEAREST,									// Point
	VK_FILTER_LINEAR,									// Linear
};

static const VkFilter vkMagTable[] =
{
	VK_FILTER_NEAREST,									// MinMagMipPoint
	VK_FILTER_LINEAR,									// MinMagMipLinear
	VK_FILTER_NEAREST,									// MinMagPointMipLinear
	VK_FILTER_LINEAR,									// MinMipPointMagLinear
	VK_FILTER_LINEAR,									// MinPointMipMagLinear
	VK_FILTER_NEAREST,									// MinLinearMipMagPoint
	VK_FILTER_NEAREST,									// MinMipLinearMagPoint
	VK_FILTER_LINEAR,									// MinMagLinearMipPoint
	VK_FILTER_LINEAR,									// Anisotropic
	VK_FILTER_NEAREST,									// Point
	VK_FILTER_LINEAR,									// Linear
};

//------------------------------------------------------------------------------
/**
*/
VkSampler::VkSampler()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkSampler::~VkSampler()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkSampler::OnLoaded()
{
	this->samplerInfo =
	{
		VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		NULL,
		0,
		vkMinTable[this->samplerSettings.filterMode],
		vkMagTable[this->samplerSettings.filterMode],
		vkMipmapTable[this->samplerSettings.filterMode],
		vkAddressModeTable[this->samplerSettings.addressU],
		vkAddressModeTable[this->samplerSettings.addressV],
		vkAddressModeTable[this->samplerSettings.addressW],
		this->samplerSettings.lodBias,
		this->samplerSettings.filterMode == Anisotropic ? true : false,
		this->samplerSettings.maxAnisotropic,
		this->samplerSettings.isComparison,
		vkComparisonTable[this->samplerSettings.comparisonFunc],
		this->samplerSettings.minLod,
		this->samplerSettings.maxLod,
		VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,	// fixme
		VK_FALSE							// fixme
	};

	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	this->bindingLayout.descriptorCount = 1;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
}

} // namespace AnyFX