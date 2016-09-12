#pragma once
//------------------------------------------------------------------------------
/**
	Implements a variable (or push constant) in Vulkan.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectvariable.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
class VkEffectProgram;
class VkEffectSampler;
class VkEffectVariable : public InternalEffectVariable
{
public:
	/// constructor
	VkEffectVariable();
	/// destructor
	virtual ~VkEffectVariable();
private:
	friend class VkEffectProgram;
	friend class VkEffectSampler;

	enum BindingType
	{
		PushConstant,
		Image
	};

	/// sets up variable from program, override in subclass
	void Setup(eastl::vector<InternalEffectProgram*> programs, const eastl::string& defaultValue);
	/// sets up variable from another variable
	void SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVariable* master);
	/// sets up texture-specific stuff
	void MakeTexture();

	/// activate this variable with the given program
	void Activate(InternalEffectProgram* program);

	/// commits changes of variable, override if required
	void Commit();

	uint32_t rangeOffset;
	bool isTexture;
	VkDescriptorSetLayoutBinding bindingLayout;
	VkDescriptorImageInfo imageInfo;
	VkSampler immutableSampler;
	VkEffectProgram* currentProgram;
};
} // namespace AnyFX
