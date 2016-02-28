//------------------------------------------------------------------------------
// vkeffectvarblock.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkeffectvarblock.h"
#include "vkeffectprogram.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VkEffectVarblock::VkEffectVarblock() :
	currentProgram(NULL)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkEffectVarblock::~VkEffectVarblock()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarblock::SetBuffer(void* handle)
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
	this->currentProgram->UpdateBufferDescriptor(this->set, this->binding, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &this->bufferInfo);
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarblock::Setup(eastl::vector<InternalEffectProgram*> programs)
{
	InternalEffectVarblock::Setup(programs);
	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	this->bindingLayout.descriptorCount = 1;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = VK_NULL_HANDLE;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarblock::SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVarblock* master)
{
	InternalEffectVarblock::SetupSlave(programs, master);
	this->bindingLayout = static_cast<VkEffectVarblock*>(master)->bindingLayout;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVarblock::Activate(InternalEffectProgram* program)
{
	this->currentProgram = static_cast<VkEffectProgram*>(program);
}

} // namespace AnyFX