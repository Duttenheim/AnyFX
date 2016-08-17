//------------------------------------------------------------------------------
// vkvariable.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkvariable.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VkVariable::VkVariable()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkVariable::~VkVariable()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkVariable::OnLoaded()
{
	VariableBase::OnLoaded();

	// add set-binding to signature
	char buf[64];
	sprintf_s(buf, sizeof(buf), "[%d-%d]", this->set, this->binding);
	this->signature += buf;

	if (this->type >= Sampler1D && this->type <= SamplerCubeArray)
	{
		this->bindingLayout.binding = this->binding;
		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		this->bindingLayout.descriptorCount = this->arraySize;
		this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
		this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
	}
	else if (this->type >= Image1D && this->type <= ImageCubeArray)
	{
		this->bindingLayout.binding = this->binding;
		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		this->bindingLayout.descriptorCount = this->arraySize;
		this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
		this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
	}
	else if (this->type >= Texture1D && this->type <= TextureCubeArray)
	{
		this->bindingLayout.binding = this->binding;
		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		this->bindingLayout.descriptorCount = this->arraySize;
		this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
		this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
	}
}

} // namespace AnyFX