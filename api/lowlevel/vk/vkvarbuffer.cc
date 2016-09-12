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

	// add set-binding to signature
	char buf[64];
	sprintf_s(buf, sizeof(buf), "[%d-%d]", this->set, this->binding);
	this->signature += buf;

	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	this->bindingLayout.descriptorCount = 1;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
}

} // namespace AnyFX