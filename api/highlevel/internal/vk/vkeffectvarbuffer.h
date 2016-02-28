#pragma once
//------------------------------------------------------------------------------
/**
	Implements a variable buffer (or shader storage buffer) in Vulkan.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectvarbuffer.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
class VkEffectVarbuffer : public InternalEffectVarbuffer
{
public:
	/// constructor
	VkEffectVarbuffer();
	/// destructor
	virtual ~VkEffectVarbuffer();
private:
	friend class VkEffectProgram;

	/// set buffer 
	void SetBuffer(void* handle);

	/// sets up varblock from program, override in subclass
	virtual void Setup(eastl::vector<InternalEffectProgram*> programs);
	/// sets up varblock from programs using a pre-existing varblock
	virtual void SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVarbuffer* master);

	/// activate this variable with the given program
	void Activate(InternalEffectProgram* program);

	VkDescriptorSetLayoutBinding bindingLayout;
	VkDescriptorBufferInfo bufferInfo;

	struct VkBufferBinding
	{
		VkBuffer buffer;
		uint32_t offset;
		uint32_t length;
	};

	VkEffectProgram* currentProgram;
};
} // namespace AnyFX