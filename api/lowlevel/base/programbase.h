#pragma once
//------------------------------------------------------------------------------
/**
	Describes a base program object as written in the AnyFX shader file.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
#include "shaderbase.h"
#include "renderstatebase.h"
namespace AnyFX
{
struct ProgramBase : public Annotable
{
public:
	/// constructor
	ProgramBase();
	/// destructor
	virtual ~ProgramBase();

	struct ShaderBlock
	{
		ShaderBase* vs;		                                        // vertex shader
		unsigned vsBinarySize;
		char* vsBinary;

		ShaderBase* hs;		                                        // hull shader
		unsigned hsBinarySize;
		char* hsBinary;

		ShaderBase* ds;		                                        // domain shader
		unsigned dsBinarySize;
		char* dsBinary;

		ShaderBase* gs;		                                        // geometry shader
		unsigned gsBinarySize;
		char* gsBinary;

		ShaderBase* ps;		                                        // pixel shader
		unsigned psBinarySize;
		char* psBinary;

		ShaderBase* cs;		                                        // compute shader
		unsigned csBinarySize;
		char* csBinary;

	} shaderBlock;

	bool supportsTessellation;
	bool supportsTransformFeedback;
	unsigned patchSize;
	eastl::string name;

	eastl::vector<eastl::string> activeVarblockNames;
	eastl::vector<eastl::string> activeVariableNames;
	eastl::map<eastl::string, unsigned> variableBlockOffsets;
	RenderStateBase* renderState;

protected:
	friend class ProgramLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX