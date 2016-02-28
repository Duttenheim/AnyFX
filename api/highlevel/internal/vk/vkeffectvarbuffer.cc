//------------------------------------------------------------------------------
// vkeffectvarbuffer.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkeffectvarbuffer.h"
#include "vkeffectprogram.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VkEffectVarbuffer::VkEffectVarbuffer() :
	currentProgram(NULL)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkEffectVarbuffer::~VkEffectVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarbuffer::SetBuffer(void* handle)
{
	if (handle != 0)
	{
		VkBufferBinding* buf = static_cast<VkBufferBinding*>(handle);
		this->bufferInfo.buffer = buf->buffer;
		this->bufferInfo.offset = buf->offset;
		this->bufferInfo.range = buf->length;
	}
	else
	{
		this->bufferInfo.buffer = 0;
	}

	// update buffer
	this->currentProgram->UpdateBufferDescriptor(this->set, this->binding, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &this->bufferInfo);
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarbuffer::Setup(eastl::vector<InternalEffectProgram*> programs)
{
	InternalEffectVarbuffer::Setup(programs);
	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	this->bindingLayout.descriptorCount = 1;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarbuffer::SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVarbuffer* master)
{
	InternalEffectVarbuffer::SetupSlave(programs, master);
	this->bindingLayout = static_cast<VkEffectVarbuffer*>(master)->bindingLayout;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarbuffer::Activate(InternalEffectProgram* program)
{
	this->currentProgram = static_cast<VkEffectProgram*>(program);
}

} // namespace AnyFX