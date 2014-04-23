#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectProgram
    
    An EffectProgram provides an interface to which we can apply shaders and render states.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#ifdef AFX_API
#error "afxapi.h included before effectprogram.h"
#endif

#include <string>
#include <vector>
#include "annotable.h"
namespace AnyFX
{
class EffectShader;
class EffectRenderState;
class InternalEffectProgram;
class EffectProgram : public Annotable
{
public:
	/// constructor
	EffectProgram();
	/// destructor
	virtual ~EffectProgram();

	/// discard program
	void Discard();

	/// applies program shaders and render states
	void Apply();
	/// commits changes to shader variables, call this before rendering
	void Commit();
	/// returns name of program
	const std::string& GetName() const;
	/// returns array to list of shaders, this is list is always a constant size of 6
	std::vector<EffectShader*> GetShaders() const;
	/// returns render state
	EffectRenderState* GetRenderState() const;

	/// returns true if the program requires patch-based rendering
	/// the OpenGL4 backend automatically sets the patch size
	const bool SupportsTessellation() const;

    /// returns the group sizes defined if the program uses a compute shader
    const unsigned* GetLocalSizes() const;

	/// returns true if linking of program was successful
	bool IsValid();
	/// returns linking error string
	const std::string& GetError() const;

private:
	friend class EffectProgramStreamLoader;
	friend class EffectVariableStreamLoader;
	friend class EffectVarblockStreamLoader;

	EffectRenderState* renderState;
	struct EffectShaderBlock
	{
		EffectShader* vs;		// vertex shader
		EffectShader* ps;		// pixel shader
		EffectShader* ds;		// domain shader
		EffectShader* hs;		// hull shader
		EffectShader* gs;		// geometry shader
		EffectShader* cs;		// compute shader
	} shaderBlock;

	int shaderMask[6];
	InternalEffectProgram* internalProgram;
}; 

//------------------------------------------------------------------------------
/**
*/
inline std::vector<EffectShader*> 
EffectProgram::GetShaders() const
{
	std::vector<EffectShader*> shaderList;
	shaderList.resize(6);
	shaderList[0] = this->shaderBlock.vs;
	shaderList[1] = this->shaderBlock.ps;
	shaderList[2] = this->shaderBlock.hs;
	shaderList[3] = this->shaderBlock.ds;
	shaderList[4] = this->shaderBlock.gs;
	shaderList[5] = this->shaderBlock.cs;
	return shaderList;
}

//------------------------------------------------------------------------------
/**
*/
inline EffectRenderState* 
EffectProgram::GetRenderState() const
{
	return this->renderState;
}


} // namespace AnyFX
//------------------------------------------------------------------------------