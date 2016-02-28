//------------------------------------------------------------------------------
// vkeffectprogram.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkeffectprogram.h"
#include "vkeffectshader.h"
#include "vkcontext.h"
#include "vkeffectrenderstate.h"
#include <assert.h>
#include "internal/internaleffectvariable.h"
#include "vkeffectvariable.h"
#include "vkeffectvarblock.h"
#include "vkeffectvarbuffer.h"
namespace AnyFX
{

bool VkEffectProgram::needsGlobalDescriptorsDiscarded;
bool VkEffectProgram::needsGlobalDescriptorsSetup;
VkPushConstantRange VkEffectProgram::globalConstantRange;
eastl::vector<VkDescriptorSetLayout> VkEffectProgram::globalDescriptorLayouts;
eastl::vector<VkDescriptorSet> VkEffectProgram::globalDescriptorSets;

//------------------------------------------------------------------------------
/**
*/
VkEffectProgram::VkEffectProgram() :
	vs(NULL),
	hs(NULL),
	ds(NULL),
	gs(NULL),
	ps(NULL),
	cs(NULL),
	stagingUpdates(NULL),
	dynamicOffsets(NULL),
	numUpdates(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkEffectProgram::~VkEffectProgram()
{
	if (this->vs) vkDestroyShaderModule(VkContext::currentContext, this->vs, NULL);
	if (this->hs) vkDestroyShaderModule(VkContext::currentContext, this->hs, NULL);
	if (this->ds) vkDestroyShaderModule(VkContext::currentContext, this->ds, NULL);
	if (this->gs) vkDestroyShaderModule(VkContext::currentContext, this->gs, NULL);
	if (this->ps) vkDestroyShaderModule(VkContext::currentContext, this->ps, NULL);
	if (this->cs) vkDestroyShaderModule(VkContext::currentContext, this->cs, NULL);
	if (this->stagingUpdates) delete[] this->stagingUpdates;
	if (this->dynamicOffsets) delete[] this->dynamicOffsets;

	// if we are allowed  to destroy the descriptor set layouts, then do so
	// this is a hack, because we really just want to have the descriptor sets stored for the program
	// and update the member variables, not go through some global shit
	if (VkEffectProgram::needsGlobalDescriptorsDiscarded)
	{
		unsigned i;
		for (i = 0; i < this->descriptorLayouts.size(); i++)
		{
			vkDestroyDescriptorSetLayout(VkContext::currentContext, this->descriptorLayouts[i], NULL);
		}
		VkEffectProgram::needsGlobalDescriptorsDiscarded = false;
	}	
}

//------------------------------------------------------------------------------
/**
	Ehh, return false if we can't possibly use this program for anything...
*/
bool
VkEffectProgram::Link()
{
	VkEffectProgram::needsGlobalDescriptorsSetup = true;

	// create shader modules
	this->CreateShader(&this->vs, this->shaderBlock.vsBinarySize, this->shaderBlock.vsBinary);
	this->CreateShader(&this->hs, this->shaderBlock.hsBinarySize, this->shaderBlock.hsBinary);
	this->CreateShader(&this->ds, this->shaderBlock.dsBinarySize, this->shaderBlock.dsBinary);
	this->CreateShader(&this->gs, this->shaderBlock.gsBinarySize, this->shaderBlock.gsBinary);
	this->CreateShader(&this->ps, this->shaderBlock.psBinarySize, this->shaderBlock.psBinary);
	this->CreateShader(&this->cs, this->shaderBlock.csBinarySize, this->shaderBlock.csBinary);
	return this->shaderBlock.vsBinarySize > 0 || this->shaderBlock.csBinarySize > 0;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::LoadingDone()
{
	InternalEffectProgram::LoadingDone();

	// if the effect creating this program is done loading, setup our 'unique' descriptors. 
	// this is hack to avoid creating more than 127 descriptor sets (which for some reason is the limit atm)
	if (VkEffectProgram::needsGlobalDescriptorsSetup) 
		VkEffectProgram::SetupDescriptors(this->varblocks, this->numVarblocks, this->varbuffers, this->numVarbuffers, this->variables, this->numVariables);
	this->descriptorLayouts = VkEffectProgram::globalDescriptorLayouts;
	this->descriptorSets = VkEffectProgram::globalDescriptorSets;
	this->constantRange = VkEffectProgram::globalConstantRange;

	// now when we have a copy of our sets and layouts, create the pipeline objects
	if (this->cs) this->CreateCompute();
	else		  this->CreateGraphics();
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::Commit()
{
	// perform updates and then bind descriptors
	vkUpdateDescriptorSets(VkContext::currentContext, this->numUpdates, this->stagingUpdates, 0, NULL);
	vkCmdBindDescriptorSets(VkContext::currentCommandBuffer, this->bindpoint, this->layout, 0, this->descriptorSets.size(), &this->descriptorSets[0], 0, NULL);
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::CreateShader(VkShaderModule* shader, unsigned binarySize, char* binary)
{
	if (binarySize > 0)
	{
		VkShaderModuleCreateInfo info =
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			NULL,
			0,										// flags
			binarySize / sizeof(unsigned),	// Vulkan expects the binary to be uint32, so we must assume size is in units of 4 bytes
			(unsigned*)binary
		};

		// create shader
		VkResult res = vkCreateShaderModule(VkContext::currentContext, &info, NULL, shader);
		assert(res == VK_SUCCESS);
	}	
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::CreateGraphics()
{
	// create 6 shader info stages for each shader type
	VkPipelineShaderStageCreateInfo shaders[6];

	// we have to keep track of how MANY shaders we are using too
	unsigned shaderIdx = 0;

	// attach vertex shader
	if (0 != this->vs)
	{
		shaders[shaderIdx].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaders[shaderIdx].pNext = NULL;
		shaders[shaderIdx].flags = 0;
		shaders[shaderIdx].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaders[shaderIdx].module = this->vs;
		shaders[shaderIdx].pName = "main";
		shaders[shaderIdx].pSpecializationInfo = NULL;
		shaderIdx++;
	}

	if (0 != this->hs)
	{
		shaders[shaderIdx].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaders[shaderIdx].pNext = NULL;
		shaders[shaderIdx].flags = 0;
		shaders[shaderIdx].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		shaders[shaderIdx].module = this->hs;
		shaders[shaderIdx].pName = "main";
		shaders[shaderIdx].pSpecializationInfo = NULL;
		shaderIdx++;
	}

	if (0 != this->ds)
	{
		shaders[shaderIdx].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaders[shaderIdx].pNext = NULL;
		shaders[shaderIdx].flags = 0;
		shaders[shaderIdx].stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		shaders[shaderIdx].module = this->ds;
		shaders[shaderIdx].pName = "main";
		shaders[shaderIdx].pSpecializationInfo = NULL;
		shaderIdx++;
	}

	if (0 != this->gs)
	{
		shaders[shaderIdx].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaders[shaderIdx].pNext = NULL;
		shaders[shaderIdx].flags = 0;
		shaders[shaderIdx].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
		shaders[shaderIdx].module = this->gs;
		shaders[shaderIdx].pName = "main";
		shaders[shaderIdx].pSpecializationInfo = NULL;
		shaderIdx++;
	}

	if (0 != this->ps)
	{
		shaders[shaderIdx].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaders[shaderIdx].pNext = NULL;
		shaders[shaderIdx].flags = 0;
		shaders[shaderIdx].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaders[shaderIdx].module = this->ps;
		shaders[shaderIdx].pName = "main";
		shaders[shaderIdx].pSpecializationInfo = NULL;
		shaderIdx++;
	}

	if (0 != this->cs)
	{
		shaders[shaderIdx].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaders[shaderIdx].pNext = NULL;
		shaders[shaderIdx].flags = 0;
		shaders[shaderIdx].stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaders[shaderIdx].module = this->cs;
		shaders[shaderIdx].pName = "main";
		shaders[shaderIdx].pSpecializationInfo = NULL;
		shaderIdx++;
	}

	// retrieve implementation specific state
	VkEffectRenderState* vkRenderState = static_cast<VkEffectRenderState*>(this->renderState);

	VkPipelineRasterizationStateCreateInfo rastInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		NULL,
	};
	vkRenderState->SetupRasterization(&rastInfo);

	VkPipelineMultisampleStateCreateInfo msInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		NULL,
	};
	vkRenderState->SetupMultisample(&msInfo);

	VkPipelineDepthStencilStateCreateInfo dsInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		NULL,
	};
	vkRenderState->SetupDepthStencil(&dsInfo);

	VkPipelineColorBlendStateCreateInfo blendInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		NULL,
	};
	vkRenderState->SetupBlend(&blendInfo);

	this->stagingUpdates = new VkWriteDescriptorSet[this->numVarblocks + this->numVarbuffers + this->numVariables];
	this->dynamicOffsets = new uint32_t[this->descriptorSets.size()];

	VkPipelineLayoutCreateInfo layoutInfo = 
	{
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		NULL,
		0,
		this->descriptorLayouts.size(),
		&this->descriptorLayouts[0],
		1,
		&this->constantRange
	};

	// create 
	VkResult res = vkCreatePipelineLayout(VkContext::currentContext, &layoutInfo, NULL, &this->layout);
	assert(res == VK_SUCCESS);

	// setup pipeline information regarding the shader state
	this->gfxPipelineInfo =
	{
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		NULL,
		VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT,
		shaderIdx,
		shaders,
		NULL, NULL, NULL, NULL,			// these are our primitive and viewport infos, keep them null to create derivatives later
		&rastInfo,
		&msInfo,
		&dsInfo,
		&blendInfo,
		NULL,							// dynamic state is also to be handled somewhere else
		this->layout, 
		NULL,							// pass specific stuff, keep as null too
		0,
		0, -1							// base pipeline is kept as 'NULL' too, because this is the base for all derivatives
	};

	// setup internal handle, don't actually create a pipeline since we don't have vertex and framebuffer info yet
	this->handle.compute = VK_NULL_HANDLE;
	this->handle.info = this->gfxPipelineInfo;
	this->handle.layout = this->layout;
	this->internalHandle = &this->handle;

	this->bindpoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::CreateCompute()
{
	// create 6 shader info stages for each shader type
	VkPipelineShaderStageCreateInfo shader;

	if (0 != this->cs)
	{
		shader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader.pNext = NULL;
		shader.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shader.module = this->cs;
	}

	this->cmpPipelineInfo =
	{
		VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		NULL,
		VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT,
		shader,
		NULL,
		NULL, -1							// base pipeline is kept as 'NULL' too, because this is the base for all derivatives
	};

	// create pipeline
	vkCreateComputePipelines(VkContext::currentContext, VkContext::currentCache, 1, &this->cmpPipelineInfo, NULL, &this->pipeline);

	this->handle.compute = this->pipeline;
	this->handle.layout = VK_NULL_HANDLE;
	this->internalHandle = &this->handle;
	this->bindpoint = VK_PIPELINE_BIND_POINT_COMPUTE;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::UpdateBufferDescriptor(uint32_t set, uint32_t binding, uint32_t offset, VkDescriptorType type, VkDescriptorBufferInfo* info)
{
	this->dynamicOffsets[this->numUpdates] = offset;
	VkWriteDescriptorSet* writeCommand = &this->stagingUpdates[this->numUpdates++];	
	writeCommand->descriptorCount = 1;
	writeCommand->descriptorType = type;
	writeCommand->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeCommand->dstSet = this->descriptorSets[set];
	writeCommand->dstBinding = binding;
	writeCommand->dstArrayElement = 0;
	writeCommand->pNext = NULL;
	writeCommand->pImageInfo = NULL;
	writeCommand->pTexelBufferView = NULL;
	writeCommand->pBufferInfo = info;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::UpdateImageDescriptor(uint32_t set, uint32_t binding, VkDescriptorType type, VkDescriptorImageInfo* info)
{
	VkWriteDescriptorSet* writeCommand = &this->stagingUpdates[this->numUpdates++];
	writeCommand->descriptorCount = 1;
	writeCommand->descriptorType = type;
	writeCommand->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeCommand->dstSet = this->descriptorSets[set];
	writeCommand->dstBinding = binding;
	writeCommand->dstArrayElement = 0;
	writeCommand->pNext = NULL;
	writeCommand->pImageInfo = info;
	writeCommand->pTexelBufferView = NULL;
	writeCommand->pBufferInfo = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectProgram::SetupDescriptors(InternalEffectVarblock** blocks, unsigned numblocks, InternalEffectVarbuffer** buffers, unsigned numbuffers, InternalEffectVariable** variables, unsigned numvariables)
{
	VkEffectProgram::needsGlobalDescriptorsSetup = false;
	VkEffectProgram::needsGlobalDescriptorsDiscarded = true;

	VkEffectProgram::globalDescriptorLayouts.clear();
	VkEffectProgram::globalDescriptorSets.clear();
	eastl::map<unsigned, eastl::vector<VkDescriptorSetLayoutBinding>> sets;
	eastl::map<unsigned, VkPushConstantRange> ranges;

#define max(a,b) (a > b ? a : b)
	unsigned numsets = 0;
	unsigned i;
	for (i = 0; i < numblocks; i++)
	{
		VkEffectVarblock* block = static_cast<VkEffectVarblock*>(blocks[i]);
		if (block->push)
		{
			// hmm, the only reason where having several push ranges is relevant is probably when using one per shader stage
			VkEffectProgram::globalConstantRange.offset = 0;
			VkEffectProgram::globalConstantRange.size = block->size;
			VkEffectProgram::globalConstantRange.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
		}
		else
		{
			if (sets.find(block->set) == sets.end()) sets[block->set] = eastl::vector<VkDescriptorSetLayoutBinding>();
			sets[block->set].push_back(block->bindingLayout);
			numsets = max(numsets, block->set);
		}
	}

	for (i = 0; i < numbuffers; i++)
	{
		VkEffectVarbuffer* buffer = static_cast<VkEffectVarbuffer*>(buffers[i]);
		if (sets.find(buffer->set) == sets.end()) sets[buffer->set] = eastl::vector<VkDescriptorSetLayoutBinding>();
		sets[buffer->set].push_back(buffer->bindingLayout);
		numsets = max(numsets, buffer->set);
	}

	// if we have 'gaps' between our groups then we still want to create enough sets so that we can set them all
	VkEffectProgram::globalDescriptorLayouts.resize(numsets);
	for (i = 0; i < numsets; i++)
	{
		VkDescriptorSetLayoutCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.pNext = NULL;
		info.flags = 0;
		if (sets.find(i) != sets.end())
		{
			const eastl::vector<VkDescriptorSetLayoutBinding>& binds = sets[i];
			info.bindingCount = binds.size();
			info.pBindings = &binds[0];
		}
		else
		{
			// no set found for this index, so we use a 'null' layout
			info.pBindings = VK_NULL_HANDLE;
			info.bindingCount = 0;
		}		

		// create layout
		VkDescriptorSetLayout layout;
		VkResult res = vkCreateDescriptorSetLayout(VkContext::currentContext, &info, NULL, &layout);
		assert(res == VK_SUCCESS);
		VkEffectProgram::globalDescriptorLayouts[i] = layout;
	}

	// allocate descriptor sets
	VkDescriptorSetAllocateInfo setInfo =
	{
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		NULL,
		VkContext::currentPool,
		VkEffectProgram::globalDescriptorLayouts.size(),
		&VkEffectProgram::globalDescriptorLayouts[0]
	};

	// allocate descriptor
	VkEffectProgram::globalDescriptorSets.resize(VkEffectProgram::globalDescriptorLayouts.size());
	VkResult res = vkAllocateDescriptorSets(VkContext::currentContext, &setInfo, &VkEffectProgram::globalDescriptorSets[0]);
	assert(res == VK_SUCCESS);
}

} // namespace AnyFX