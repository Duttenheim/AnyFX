#pragma once
//------------------------------------------------------------------------------
/**
	Describes the basics of a variable block.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/varblockbase.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
struct VkVarblock : public VarblockBase
{
public:
	/// constructor
	VkVarblock();
	/// destructor
	virtual ~VkVarblock();

	VkDescriptorSetLayoutBinding bindingLayout;
private:

	/// setup binding layout
	void OnLoaded();
};
} // namespace AnyFX