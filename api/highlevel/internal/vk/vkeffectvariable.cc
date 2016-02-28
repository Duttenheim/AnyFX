//------------------------------------------------------------------------------
// vkeffectvariable.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "vkeffectvariable.h"
#include "vkeffectprogram.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VkEffectVariable::VkEffectVariable() :
	isTexture(false),
	rangeOffset(0),
	immutableSampler(VK_NULL_HANDLE)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkEffectVariable::~VkEffectVariable()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVariable::Setup(eastl::vector<InternalEffectProgram*> programs, const eastl::string& defaultValue)
{
	InternalEffectVariable::Setup(programs, defaultValue);
	// only fetch the shared byte offset if we are in a varblock
	this->sharedByteOffset = new unsigned;
	if (this->isInVarblock)
	{
		unsigned i;
		for (i = 0; i < programs.size(); i++)
		{
			const VkEffectProgram* prog = static_cast<VkEffectProgram*>(programs[i]);
			eastl::map<eastl::string, unsigned>::const_iterator it = prog->variableBlockOffsets.find(this->name.c_str());
			if (it != prog->variableBlockOffsets.end())
			{
				*this->sharedByteOffset = it->second;
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVariable::SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVariable* master)
{
	InternalEffectVariable::SetupSlave(programs, master);
	this->sharedByteOffset = static_cast<VkEffectVariable*>(master)->sharedByteOffset;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVariable::MakeTexture()
{
	this->bindingLayout.binding = this->binding;
	this->bindingLayout.descriptorCount = this->arraySize;
	this->bindingLayout.stageFlags = VK_SHADER_STAGE_ALL;
	this->bindingLayout.pImmutableSamplers = &this->immutableSampler;

	if (this->type >= Sampler1D && this->type <= SamplerCubeArray)		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	else if (this->type >= Image1D && this->type <= ImageCubeArray)		this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	else if (this->type >= Texture1D && this->type <= TextureCubeArray) this->bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	this->isTexture = true;
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVariable::Activate(InternalEffectProgram* program)
{
	this->currentProgram = static_cast<VkEffectProgram*>(program);
}

//------------------------------------------------------------------------------
/**
*/
void
VkEffectVariable::Commit()
{
	if (this->isSubroutine) return;
	if (!this->isInVarblock && this->isDirty)
	{
		switch (this->type)
		{
		case Sampler1D:
		case Sampler1DArray:
		case Sampler2D:
		case Sampler2DArray:
		case Sampler2DMS:
		case Sampler2DMSArray:
		case Sampler3D:
		case SamplerCube:
		case SamplerCubeArray:
			break;

		case Image1D:
		case Image2D:
		case Image2DMS:
		case Image1DArray:
		case Image2DArray:
		case Image2DMSArray:
		case Image3D:
		case ImageCube:
		case ImageCubeArray:
			break;

		case Texture1D:
		case Texture2D:
		case Texture2DMS:
		case Texture1DArray:
		case Texture2DArray:
		case Texture2DMSArray:
		case Texture3D:
		case TextureCube:
		case TextureCubeArray:
			break;

		default:
			
			break;
		}

		// uncheck dirty flag
		this->isDirty = false;
	}
}

} // namespace AnyFX