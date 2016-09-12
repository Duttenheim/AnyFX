//------------------------------------------------------------------------------
// vkvariable.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkvariable.h"
#if __WIN32__
#define snprintf sprintf_s
#endif

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
	snprintf(buf, sizeof(buf), "[%d-%d]", this->set, this->binding);
	this->signature += buf;

	if (this->type >= Sampler1D && this->type <= SamplerCubeArray)
	{
		this->bindingLayout.binding = this->binding;
		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		this->bindingLayout.descriptorCount = this->arraySize;
		this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
		this->bindingLayout.pImmutableSamplers = NULL;
	}
	else if (this->type >= Image1D && this->type <= ImageCubeArray)
	{
		this->bindingLayout.binding = this->binding;
		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		this->bindingLayout.descriptorCount = this->arraySize;
		this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
		this->bindingLayout.pImmutableSamplers = NULL;
	}
	else if (this->type >= Texture1D && this->type <= TextureCubeArray)
	{
		this->bindingLayout.binding = this->binding;
		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		this->bindingLayout.descriptorCount = this->arraySize;
		this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
		this->bindingLayout.pImmutableSamplers = NULL;
	}
	else if (this->type == InputAttachment)
	{
		this->bindingLayout.binding = this->binding;
		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		this->bindingLayout.descriptorCount = this->arraySize;
		this->bindingLayout.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		this->bindingLayout.pImmutableSamplers = NULL;
	}
}

} // namespace AnyFX
