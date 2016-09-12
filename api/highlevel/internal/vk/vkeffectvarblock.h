#pragma once
//------------------------------------------------------------------------------
/**
	Implements a variable block (or uniform buffer binding slot) in Vulkan.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectvarblock.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
class VkEffectProgram;
class VkEffectVarblock : public InternalEffectVarblock
{
public:
	/// constructor
	VkEffectVarblock();
	/// destructor
	virtual ~VkEffectVarblock();
private:
	friend class VkEffectProgram;

	/// set buffer 
	void SetBuffer(void* handle);

	/// sets up varblock from program, override in subclass
	void Setup(eastl::vector<InternalEffectProgram*> programs);
	/// sets up varblock from programs using a pre-existing varblock
	void SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVarblock* master);

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
