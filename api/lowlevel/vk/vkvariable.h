#pragma once
//------------------------------------------------------------------------------
/**
	Describes the base for a variable.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/variablebase.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
struct VkVariable : public VariableBase
{
public:
	/// constructor
	VkVariable();
	/// destructor
	virtual ~VkVariable();

	VkDescriptorSetLayoutBinding bindingLayout;
private:

	/// setup binding layout
	void OnLoaded();
};
} // namespace AnyFX