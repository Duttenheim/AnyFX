#pragma once
//------------------------------------------------------------------------------
/**
	Implements a Vulkan graphics pipeline using shaders and render state.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectprogram.h"
#include <vulkan/vulkan.h>

namespace AnyFX
{
class VkEffectProgram : public InternalEffectProgram
{
public:
	/// constructor
	VkEffectProgram();
	/// destructor
	virtual ~VkEffectProgram();

	struct VkProgramHandle
	{
		VkGraphicsPipelineCreateInfo info;
		VkPipelineLayout layout;
		VkPipeline compute;
	} handle;
private:
	friend class VkEffectVarblock;
	friend class VkEffectVarbuffer;
	friend class VkEffectVariable;
	/// links program
	bool Link();
	/// perform pipeline setup when loading of the shader file has finished
	void LoadingDone();

	/// commits variables
	void Commit();

	/// create shader object
	void CreateShader(VkShaderModule* shader, unsigned binarySize, char* binary);
	/// create program as graphics
	void CreateGraphics();
	/// create program as compute
	void CreateCompute();

	/// update descriptor binding slot for buffers
	void UpdateBufferDescriptor(uint32_t set, uint32_t binding, uint32_t offset, VkDescriptorType type, VkDescriptorBufferInfo* info);
	/// update descriptor binding slot for textures
	void UpdateImageDescriptor(uint32_t set, uint32_t binding, VkDescriptorType type, VkDescriptorImageInfo* info);
	/// static function used to update the static set of descriptors used by this program
	static void SetupDescriptors(InternalEffectVarblock** blocks, unsigned numblocks, InternalEffectVarbuffer** buffers, unsigned numbuffers, InternalEffectVariable** variables, unsigned numvariables);

	VkShaderModule vs, hs, ds, gs, ps, cs;
	
	VkPipeline pipeline;
	VkPipelineLayout layout;
	VkPipelineBindPoint bindpoint;

	static bool needsGlobalDescriptorsDiscarded;
	static bool needsGlobalDescriptorsSetup;
	static VkPushConstantRange globalConstantRange;
	static eastl::vector<VkDescriptorSetLayout> globalDescriptorLayouts;
	static eastl::vector<VkDescriptorSet> globalDescriptorSets;

	VkPushConstantRange constantRange;
	eastl::vector<VkDescriptorSetLayout> descriptorLayouts;
	eastl::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSet bindSet[64];


	VkGraphicsPipelineCreateInfo gfxPipelineInfo;
	VkComputePipelineCreateInfo cmpPipelineInfo;

	uint32_t numUpdates;
	VkWriteDescriptorSet* stagingUpdates;
	uint32_t* dynamicOffsets;
};
} // namespace Vulkan