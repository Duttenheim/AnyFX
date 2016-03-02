#pragma once
//------------------------------------------------------------------------------
/**
	Implements a basic variable buffer.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/varbufferbase.h"
#include <vulkan/vulkan.h>
namespace AnyFX
{
struct VkVarbuffer : public VarbufferBase
{
public:
	/// constructor
	VkVarbuffer();
	/// destructor
	virtual ~VkVarbuffer();

	VkDescriptorSetLayoutBinding bindingLayout;
private:

	/// setup binding layout
	void OnLoaded();
};
} // namespace AnyFX