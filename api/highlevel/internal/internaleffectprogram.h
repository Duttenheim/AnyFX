#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::InternalEffectProgram
    
    EffectProgram backend, inherit this class to provide an implementation
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "EASTL/map.h"
namespace AnyFX
{
class Effect;
class InternalEffectRenderState;
class InternalEffectShader;
class InternalEffectSubroutine;
class InternalEffectVariable;
class InternalEffectVarblock;
class InternalEffectVarbuffer;
class InternalEffectSampler;
class InternalEffectProgram
{
public:

	enum LinkState
	{
		NotLinked,
		LinkedOk,
		LinkerError
	};
	/// constructor
	InternalEffectProgram();
	/// destructor
	virtual ~InternalEffectProgram();

protected:
	friend class EffectProgram;
	friend class EffectProgramStreamLoader;
	friend class EffectStreamLoader;

	/// applies program, override in subclass to provide functionality
	virtual void Apply();
	/// commits variables
	virtual void Commit();
	/// links program, override in subclass if needed
	virtual bool Link();
	/// returns true if program supports tessellation
	const bool SupportsTessellation() const;
	/// get handle to internal object
	void* GetInternalHandle();

	/// copy internal handle from other program
	virtual void SetupSlave(InternalEffectProgram* other);

    /// sets up subroutine mappings
    virtual void SetupSubroutines();

	/// returns name of program
    const eastl::string& GetName() const;
	/// returns error, if any
    const eastl::string& GetError() const;
	/// returns warning string
    const eastl::string& GetWarning() const;

	/// notification that loading is done
	virtual void LoadingDone();

	InternalEffectRenderState* renderState;
	Effect* effect;

	struct InternalEffectShaderBlock
	{
		InternalEffectShader* vs;		                                        // vertex shader
        eastl::map<eastl::string, InternalEffectSubroutine*> vsSubroutines;     
		unsigned vsBinarySize;
		char* vsBinary;

		InternalEffectShader* hs;		                                        // hull shader
		eastl::map<eastl::string, InternalEffectSubroutine*> hsSubroutines;     
		unsigned hsBinarySize;
		char* hsBinary;

		InternalEffectShader* ds;		                                        // domain shader
		eastl::map<eastl::string, InternalEffectSubroutine*> dsSubroutines;     
		unsigned dsBinarySize;
		char* dsBinary;

		InternalEffectShader* gs;		                                        // geometry shader
		eastl::map<eastl::string, InternalEffectSubroutine*> gsSubroutines;     
		unsigned gsBinarySize;
		char* gsBinary;

		InternalEffectShader* ps;		                                        // pixel shader
        eastl::map<eastl::string, InternalEffectSubroutine*> psSubroutines;     
		unsigned psBinarySize;
		char* psBinary;

		InternalEffectShader* cs;		                                        // compute shader
        eastl::map<eastl::string, InternalEffectSubroutine*> csSubroutines;     
		unsigned csBinarySize;
		char* csBinary;

	} shaderBlock;

	InternalEffectVariable** variables;
	unsigned numVariables;

	InternalEffectVarblock** varblocks;
	unsigned numVarblocks;

	InternalEffectVarbuffer** varbuffers;
	unsigned numVarbuffers;

	eastl::map<eastl::string, unsigned> variableBlockOffsets;

	InternalEffectSampler** samplers;
	unsigned numSamplers;

	eastl::string name;
	eastl::string error;
	eastl::string warning;

	LinkState linkState;
	bool supportsTessellation;
    bool supportsTransformFeedback;
	unsigned patchSize;
	unsigned numVsInputs;
	eastl::vector<unsigned> vsInputSlots;
	unsigned numPsOutputs;
	eastl::vector<unsigned> psOutputSlots;

	void* internalHandle;
}; 

//------------------------------------------------------------------------------
/**
*/
inline const eastl::string&
InternalEffectProgram::GetName() const
{
	return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline const eastl::string&
InternalEffectProgram::GetError() const
{
	return this->error;
}

//------------------------------------------------------------------------------
/**
*/
inline const eastl::string&
InternalEffectProgram::GetWarning() const
{
	return this->warning;
}

//------------------------------------------------------------------------------
/**
*/
inline const bool 
InternalEffectProgram::SupportsTessellation() const
{
	return this->supportsTessellation;
}

//------------------------------------------------------------------------------
/**
*/
inline void*
InternalEffectProgram::GetInternalHandle()
{
	return this->internalHandle;
}

} // namespace AnyFX
//------------------------------------------------------------------------------
