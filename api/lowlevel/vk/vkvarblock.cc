//------------------------------------------------------------------------------
// vkvarblock.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkvarblock.h"
#if __WIN32__
#define snprintf sprintf_s
#endif

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

	// add set-binding to signature
	char buf[64];
	snprintf(buf, sizeof(buf), "[%d-%d]", this->set, this->binding);
	this->signature += buf;

	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	this->bindingLayout.descriptorCount = 1;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
}

} // namespace AnyFX
