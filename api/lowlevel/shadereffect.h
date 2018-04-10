#pragma once
//------------------------------------------------------------------------------
/**
	Implements the main entry point for the lowlevel API for the shading system.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vector>
#include <string>
#include "shadertypes.h"
#include "base/programbase.h"
#include "base/shaderbase.h"
#include "base/renderstatebase.h"
#include "base/variablebase.h"
#include "base/varblockbase.h"
#include "base/varbufferbase.h"
#include "base/samplerbase.h"
#include "base/subroutinebase.h"
namespace AnyFX
{
class ShaderEffect
{
public:
	/// constructor
	ShaderEffect();
	/// destructor
	virtual ~ShaderEffect();

	/// returns number of programs
	unsigned GetNumPrograms() const;
	/// returns program by index
	ProgramBase* GetProgram(const unsigned i) const;
	/// returns program by name
	ProgramBase* GetProgram(const std::string& name) const;
	/// returns all programs as a list
	const std::vector<ProgramBase*>& GetPrograms() const;
	/// returns true if program exists
	bool HasProgram(const std::string& name) const;

	/// returns number of shaders
	unsigned GetNumShaders() const;
	/// returns shader by index
	ShaderBase* GetShader(const unsigned i) const;
	/// returns shader by name
	ShaderBase* GetShader(const std::string& name) const;
	/// returns shaders as a list
	const std::vector<ShaderBase*>& GetShaders() const;
	/// returns true if shader exists
	bool HasShader(const std::string& name) const;

	/// returns number of render states
	unsigned GetNumRenderStates() const;
	/// returns render state by index
	RenderStateBase* GetRenderState(const unsigned i) const;
	/// returns render state by name
	RenderStateBase* GetRenderState(const std::string& name) const;
	/// returns render states as a list
	const std::vector<RenderStateBase*>& GetRenderStates() const;
	/// returns true if render state exists
	bool HasRenderState(const std::string& name) const;

	/// returns number of variables
	unsigned GetNumVariables() const;
	/// returns variable by index
	VariableBase* GetVariable(const unsigned i) const;
	/// returns variable by name
	VariableBase* GetVariable(const std::string& name) const;
	/// returns variables as a list
	const std::vector<VariableBase*>& GetVariables() const;
	/// returns varbuffers based on group
	const std::vector<VariableBase*>& GetVariables(const unsigned group) const;
	/// returns true if variable exists
	bool HasVariable(const std::string& name) const;
	/// returns true if variables for given group exists
	bool HasVariables(const unsigned group) const;

	/// returns number of varblocks
	unsigned GetNumVarblocks() const;
	/// returns varblock by index
	VarblockBase* GetVarblock(const unsigned i) const;
	/// returns varblock by name
	VarblockBase* GetVarblock(const std::string& name) const;
	/// returns varblocks as a list
	const std::vector<VarblockBase*>& GetVarblocks() const;
	/// returns varbuffers based on group
	const std::vector<VarblockBase*>& GetVarblocks(const unsigned group) const;
	/// returns true if varblock exists
	bool HasVarblock(const std::string& name) const;
	/// returns true if varblocks for given group exists
	bool HasVarblocks(const unsigned group) const;

	/// returns number of varbuffers
	unsigned GetNumVarbuffers() const;
	/// returns varbuffer by index
	VarbufferBase* GetVarbuffer(const unsigned i) const;
	/// returns varbuffer by name
	VarbufferBase* GetVarbuffer(const std::string& name) const;
	/// returns varbuffer as a list
	const std::vector<VarbufferBase*>& GetVarbuffers() const;
	/// returns varbuffers based on group
	const std::vector<VarbufferBase*>& GetVarbuffers(const unsigned group) const;
	/// returns true if varbuffer exists
	bool HasVarbuffer(const std::string& name) const;
	/// returns true if varbuffers for given group exists
	bool HasVarbuffers(const unsigned group) const;

	/// returns number of subroutine
	unsigned GetNumSubroutines() const;
	/// returns subroutine by index
	SubroutineBase* GetSubroutineByName(const unsigned i) const;
	/// returns subroutine by name
	SubroutineBase* GetSubroutineByName(const std::string& name) const;
	/// returns list of subroutines
	const std::vector<SubroutineBase*>& GetSubroutines() const;
	/// returns true if subroutine exists
	bool HasSubroutine(const std::string& name) const;

	/// returns the number of samplers
	unsigned GetNumSamplers() const;
	/// returns sampler by index
	SamplerBase* GetSampler(const unsigned i) const;
	/// returns sampler by name
	SamplerBase* GetSampler(const std::string& name) const;
	/// returns samplers as list
	const std::vector<SamplerBase*>& GetSamplers() const;
	/// returns true if sampler exists
	bool HasSampler(const std::string& name) const;
private:
	friend class StreamLoader;
	friend class VarblockLoader;
	friend class ProgramLoader;
	friend class VariableLoader;
	friend class VarbufferLoader;
	friend class RenderStateLoader;
	friend class SubroutineLoader;
	friend class ShaderLoader;
	friend class SamplerLoader;


	Implementation header;
	unsigned major;
	unsigned minor;

	std::map<std::string, ProgramBase*> programs;
	std::vector<ProgramBase*> programsByIndex;

	std::map<std::string, ShaderBase*> shaders;
	std::vector<ShaderBase*> shadersByIndex;

	std::map<std::string, VariableBase*> variables;
	std::map<unsigned, std::vector<VariableBase*>> variablesByGroup;
	std::vector<VariableBase*> variablesByIndex;

	std::map<std::string, RenderStateBase*> renderstates;
	std::vector<RenderStateBase*> renderstatesByIndex;

	std::map<std::string, SubroutineBase*> subroutines;
	std::vector<SubroutineBase*> subroutinesByIndex;

	std::map<std::string, VarblockBase*> varblocks;
	std::map<unsigned, std::vector<VarblockBase*>> varblocksByGroup;
	std::vector<VarblockBase*> varblocksByIndex;

	std::map<std::string, VarbufferBase*> varbuffers;
	std::map<unsigned, std::vector<VarbufferBase*>> varbuffersByGroup;
	std::vector<VarbufferBase*> varbuffersByIndex;

	std::map<std::string, SamplerBase*> samplers;
	std::vector<SamplerBase*> samplersByIndex;
};
} // namespace AnyFX