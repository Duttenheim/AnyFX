//------------------------------------------------------------------------------
// vkvarbuffer.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkvarbuffer.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VkVarbuffer::VkVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkVarbuffer::~VkVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkVarbuffer::OnLoaded()
{
	VarbufferBase::OnLoaded();
	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	this->bindingLayout.descriptorCount = 1;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
}

} // namespace AnyFX