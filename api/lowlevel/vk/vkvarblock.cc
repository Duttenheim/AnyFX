//------------------------------------------------------------------------------
// vkvarblock.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkvarblock.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VkVarblock::VkVarblock()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkVarblock::~VkVarblock()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkVarblock::OnLoaded()
{
	VarblockBase::OnLoaded();
	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	this->bindingLayout.descriptorCount = 1;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
}

} // namespace AnyFX