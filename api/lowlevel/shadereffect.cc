//------------------------------------------------------------------------------
// shadereffect.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "shadereffect.h"
#include <assert.h>

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
ShaderEffect::ShaderEffect()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderEffect::~ShaderEffect()
{
	unsigned i;
	for (i = 0; i < this->programsByIndex.size(); i++) delete this->programsByIndex[i];
	for (i = 0; i < this->shadersByIndex.size(); i++) delete this->shadersByIndex[i];
	for (i = 0; i < this->variablesByIndex.size(); i++) delete this->variablesByIndex[i];
	for (i = 0; i < this->renderstatesByIndex.size(); i++) delete this->renderstatesByIndex[i];
	for (i = 0; i < this->subroutinesByIndex.size(); i++) delete this->subroutinesByIndex[i];
	for (i = 0; i < this->varbuffersByIndex.size(); i++) delete this->varbuffersByIndex[i];
	for (i = 0; i < this->samplersByIndex.size(); i++) delete this->samplersByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumPrograms() const
{
	return this->programs.size();
}

//------------------------------------------------------------------------------
/**
*/
ProgramBase*
ShaderEffect::GetProgram(const eastl::string& name)
{
	assert(this->programs.find(name) != this->programs.end());
	return this->programs[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<ProgramBase*>&
ShaderEffect::GetPrograms() const
{
	return this->programsByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasProgram(const eastl::string& name)
{
	return this->programs.find(name) != this->programs.end();
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumShaders() const
{
	return this->shaders.size();
}

//------------------------------------------------------------------------------
/**
*/
ShaderBase*
ShaderEffect::GetShader(const eastl::string& name)
{
	assert(this->shaders.find(name) != this->shaders.end());
	return this->shaders[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<ShaderBase*>&
ShaderEffect::GetShaders() const
{
	return this->shadersByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasShader(const eastl::string& name)
{
	return this->shaders.find(name) != this->shaders.end();
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumRenderStates() const
{
	return this->renderstates.size();
}

//------------------------------------------------------------------------------
/**
*/
RenderStateBase*
ShaderEffect::GetRenderState(const eastl::string& name)
{
	assert(this->renderstates.find(name) != this->renderstates.end());
	return this->renderstates[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<RenderStateBase*>&
ShaderEffect::GetRenderStates() const
{
	return this->renderstatesByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasRenderState(const eastl::string& name)
{
	return this->renderstates.find(name) != this->renderstates.end();
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumVariables() const
{
	return this->variables.size();
}

//------------------------------------------------------------------------------
/**
*/
VariableBase*
ShaderEffect::GetVariable(const eastl::string& name)
{
	assert(this->variables.find(name) != this->variables.end());
	return this->variables[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<VariableBase*>&
ShaderEffect::GetVariables() const
{
	return this->variablesByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasVariable(const eastl::string& name)
{
	return this->variables.find(name) != this->variables.end();
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumVarblocks() const
{
	return this->varblocks.size();
}

//------------------------------------------------------------------------------
/**
*/
VarblockBase*
ShaderEffect::GetVarblock(const eastl::string& name)
{
	assert(this->varblocks.find(name) != this->varblocks.end());
	return this->varblocks[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<VarblockBase*>&
ShaderEffect::GetVarblocks() const
{
	return this->varblocksByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasVarblock(const eastl::string& name)
{
	return this->varblocks.find(name) != this->varblocks.end();
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumVarbuffers() const
{
	return this->varbuffers.size();
}

//------------------------------------------------------------------------------
/**
*/
VarbufferBase*
ShaderEffect::GetVarbuffer(const eastl::string& name)
{
	assert(this->varbuffers.find(name) != this->varbuffers.end());
	return this->varbuffers[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<VarbufferBase*>&
ShaderEffect::GetVarbuffers() const
{
	return this->varbuffersByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasVarbuffer(const eastl::string& name)
{
	return this->varbuffers.find(name) != this->varbuffers.end();
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumSubroutines() const
{
	return this->subroutines.size();
}

//------------------------------------------------------------------------------
/**
*/
SubroutineBase*
ShaderEffect::GetSubroutineByName(const eastl::string& name)
{
	assert(this->subroutines.find(name) != this->subroutines.end());
	return this->subroutines[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<SubroutineBase*>&
ShaderEffect::GetSubroutines() const
{
	return this->subroutinesByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasSubroutine(const eastl::string& name)
{
	return this->subroutines.find(name) != this->subroutines.end();
}

//------------------------------------------------------------------------------
/**
*/
unsigned
ShaderEffect::GetNumSamplers() const
{
	return this->samplers.size();
}

//------------------------------------------------------------------------------
/**
*/
SamplerBase*
ShaderEffect::GetSampler(const eastl::string& name)
{
	assert(this->samplers.find(name) != this->samplers.end());
	return this->samplers[name];
}

//------------------------------------------------------------------------------
/**
*/
const eastl::vector<SamplerBase*>&
ShaderEffect::GetSamplers() const
{
	return this->samplersByIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasSampler(const eastl::string& name)
{
	return this->samplers.find(name) != this->samplers.end();
}

} // namespace AnyFX