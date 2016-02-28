#pragma once
//------------------------------------------------------------------------------
/**
	Implements the main entry point for the lowlevel API for the shading system.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <EASTL/vector.h>
#include <EASTL/string.h>
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
	/// returns program by name
	ProgramBase* GetProgram(const eastl::string& name);
	/// returns all programs as a list
	const eastl::vector<ProgramBase*>& GetPrograms() const;
	/// returns true if program exists
	bool HasProgram(const eastl::string& name);

	/// returns number of shaders
	unsigned GetNumShaders() const;
	/// returns shader by name
	ShaderBase* GetShader(const eastl::string& name);
	/// returns shaders as a list
	const eastl::vector<ShaderBase*>& GetShaders() const;
	/// returns true if shader exists
	bool HasShader(const eastl::string& name);

	/// returns number of render states
	unsigned GetNumRenderStates() const;
	/// returns render state by name
	RenderStateBase* GetRenderState(const eastl::string& name);
	/// returns render states as a list
	const eastl::vector<RenderStateBase*>& GetRenderStates() const;
	/// returns true if render state exists
	bool HasRenderState(const eastl::string& name);

	/// returns number of variables
	unsigned GetNumVariables() const;
	/// returns variable by name
	VariableBase* GetVariable(const eastl::string& name);
	/// returns variables as a list
	const eastl::vector<VariableBase*>& GetVariables() const;
	/// returns true if variable exists
	bool HasVariable(const eastl::string& name);

	/// returns number of varblocks
	unsigned GetNumVarblocks() const;
	/// returns varblock by name
	VarblockBase* GetVarblock(const eastl::string& name);
	/// returns varblocks as a list
	const eastl::vector<VarblockBase*>& GetVarblocks() const;
	/// returns true if varblock exists
	bool HasVarblock(const eastl::string& name);

	/// returns number of varbuffers
	unsigned GetNumVarbuffers() const;
	/// returns varbuffer by name
	VarbufferBase* GetVarbuffer(const eastl::string& name);
	/// returns varbuffer as a list
	const eastl::vector<VarbufferBase*>& GetVarbuffers() const;
	/// returns true if varbuffer exists
	bool HasVarbuffer(const eastl::string& name);

	/// returns number of subroutine
	unsigned GetNumSubroutines() const;
	/// returns subroutine by name
	SubroutineBase* GetSubroutineByName(const eastl::string& name);
	/// returns list of subroutines
	const eastl::vector<SubroutineBase*>& GetSubroutines() const;
	/// returns true if subroutine exists
	bool HasSubroutine(const eastl::string& name);

	/// returns the number of samplers
	unsigned GetNumSamplers() const;
	/// returns sampler by name
	SamplerBase* GetSampler(const eastl::string& name);
	/// returns samplers as list
	const eastl::vector<SamplerBase*>& GetSamplers() const;
	/// returns true if sampler exists
	bool HasSampler(const eastl::string& name);
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

	eastl::map<eastl::string, ProgramBase*> programs;
	eastl::map<eastl::string, ShaderBase*> shaders;
	eastl::map<eastl::string, VariableBase*> variables;
	eastl::map<eastl::string, RenderStateBase*> renderstates;
	eastl::map<eastl::string, SubroutineBase*> subroutines;
	eastl::map<eastl::string, VarblockBase*> varblocks;
	eastl::map<eastl::string, VarbufferBase*> varbuffers;
	eastl::map<eastl::string, SamplerBase*> samplers;
};
} // namespace AnyFX