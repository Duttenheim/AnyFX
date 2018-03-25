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
ShaderEffect::GetProgram(const unsigned i) const
{
	return this->programsByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
ProgramBase*
ShaderEffect::GetProgram(const eastl::string& name) const
{
	const auto it = this->programs.find(name);
	assert(it != this->programs.end());
	return it->second;
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
ShaderEffect::HasProgram(const eastl::string& name) const
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
ShaderEffect::GetShader(const unsigned i) const
{
	return this->shadersByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
ShaderBase*
ShaderEffect::GetShader(const eastl::string& name) const
{
	const auto it = this->shaders.find(name);
	assert(it != this->shaders.end());
	return it->second;
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
ShaderEffect::HasShader(const eastl::string& name) const
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
ShaderEffect::GetRenderState(const unsigned i) const
{
	return this->renderstatesByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
RenderStateBase*
ShaderEffect::GetRenderState(const eastl::string& name) const
{
	const auto it = this->renderstates.find(name);
	assert(it != this->renderstates.end());
	return it->second;
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
ShaderEffect::HasRenderState(const eastl::string& name) const
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
ShaderEffect::GetVariable(const unsigned i) const
{
	return this->variablesByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
VariableBase*
ShaderEffect::GetVariable(const eastl::string& name) const
{
	const auto it = this->variables.find(name);
	assert(it != this->variables.end());
	return it->second;
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
const eastl::vector<VariableBase*>&
ShaderEffect::GetVariables(const unsigned group) const
{
	const auto it = this->variablesByGroup.find(group);
	assert(it != this->variablesByGroup.end());
	return it->second;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasVariable(const eastl::string& name) const
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
ShaderEffect::GetVarblock(const unsigned i) const
{
	return this->varblocksByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
VarblockBase*
ShaderEffect::GetVarblock(const eastl::string& name) const
{
	const auto it = this->varblocks.find(name);
	assert(it != this->varblocks.end());
	return it->second;
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
const eastl::vector<VarblockBase*>&
ShaderEffect::GetVarblocks(const unsigned group) const
{
	const auto it = this->varblocksByGroup.find(group);
	assert(it != this->varblocksByGroup.end());
	return it->second;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasVarblock(const eastl::string& name) const
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
ShaderEffect::GetVarbuffer(const unsigned i) const
{
	return this->varbuffersByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
VarbufferBase*
ShaderEffect::GetVarbuffer(const eastl::string& name) const
{
	const auto it = this->varbuffers.find(name);
	assert(it != this->varbuffers.end());
	return it->second;
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
const eastl::vector<VarbufferBase*>&
ShaderEffect::GetVarbuffers(const unsigned group) const
{
	const auto it = this->varbuffersByGroup.find(group);
	assert(it != this->varbuffersByGroup.end());
	return it->second;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderEffect::HasVarbuffer(const eastl::string& name) const
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
ShaderEffect::GetSubroutineByName(const unsigned i) const
{
	return this->subroutinesByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
SubroutineBase*
ShaderEffect::GetSubroutineByName(const eastl::string& name) const
{
	const auto it = this->subroutines.find(name);
	assert(it != this->subroutines.end());
	return it->second;
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
ShaderEffect::HasSubroutine(const eastl::string& name) const
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
ShaderEffect::GetSampler(const unsigned i) const
{
	return this->samplersByIndex[i];
}

//------------------------------------------------------------------------------
/**
*/
SamplerBase*
ShaderEffect::GetSampler(const eastl::string& name) const
{
	const auto it = this->samplers.find(name);
	assert(it != this->samplers.end());
	return it->second;
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
ShaderEffect::HasSampler(const eastl::string& name) const
{
	return this->samplers.find(name) != this->samplers.end();
}

} // namespace AnyFX