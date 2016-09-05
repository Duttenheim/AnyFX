#pragma once
//------------------------------------------------------------------------------
/**
    @class Effect
    
    Main parser entry point, contains complete effect.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <vector>
#include <map>
#include "header.h"
#include "program.h"
#include "variable.h"
#include "renderstate.h"
#include "function.h"
#include "structure.h"
#include "varblock.h"
#include "varbuffer.h"
#include "subroutine.h"
#include "compileable.h"
#include "sampler.h"

namespace AnyFX
{

class Effect : public Compileable
{
public:

	/// constructor
	Effect();
	/// destructor
	virtual ~Effect();

	/// sets the header object
	void SetHeader(const Header& header);
	/// gets the header object
	const Header& GetHeader() const;

	/// add program definition
	void AddProgram(const Program& program);
	/// add variable definition
	void AddVariable(const Variable& var);
	/// add constant definition
	void AddConstant(const Constant& constant);
	/// add render state
	void AddRenderState(const RenderState& state);
	/// add function
	void AddFunction(const Function& function);
	/// add structure
	void AddStructure(const Structure& structure);
	/// add varblock
	void AddVarBlock(const VarBlock& varBlock);
    /// add varbuffer
    void AddVarBuffer(const VarBuffer& varBuffer);
    /// add subroutine
    void AddSubroutine(const Subroutine& subroutine);
	/// add sampler
	void AddSampler(const Sampler& sampler);

	/// set the vector of passthrough macros
	void SetPreprocessorPassthrough(const std::vector<std::string>& pps);

	/// sets up effect using given header
	void Setup();
	/// type check effect
	void TypeCheck(TypeChecker& typechecker);
	/// generates code
	void Generate(Generator& generator);
	/// compile effect
	void Compile(BinWriter& writer);

	/// set debug output
	void SetDebugOutputPath(const std::string& debugOutput);

	/// align value to nearest power of two
	static unsigned AlignToPow(unsigned num, unsigned pow);
	/// calculate offset of variable using shared/std140 declared in GLSL/SPIR-V
	static unsigned GetAlignmentGLSL(const DataType& type, unsigned arraySize, unsigned& alignedSize, unsigned& stride, unsigned& elementStride, std::vector<unsigned>& suboffsets, const bool& std140, TypeChecker& typechecker);

private:
	Header header;
	std::vector<Program> programs;
	std::vector<Variable> variables;
	std::vector<Constant> constants;
	std::vector<RenderState> renderStates;
	std::vector<Function> functions;
	std::vector<Structure> structures;
	std::vector<VarBlock> varBlocks;
    std::vector<VarBuffer> varBuffers;
    std::vector<Subroutine> subroutines;
	std::vector<Sampler> samplers;

	std::vector<std::string> passthroughPPs;
	std::map<std::string, Shader*> shaders;

	RenderState placeholderRenderState;
    VarBlock placeholderVarBlock;

	std::string debugOutput;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void 
Effect::SetHeader(const Header& header)
{
	this->header = header;
}

//------------------------------------------------------------------------------
/**
*/
inline const Header&
Effect::GetHeader() const
{
	return this->header;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Effect::SetDebugOutputPath(const std::string& debugOutput)
{
	this->debugOutput = debugOutput;
}

//------------------------------------------------------------------------------
/**
*/
inline unsigned
Effect::AlignToPow(unsigned num, unsigned pow)
{
	return ((num + pow - 1) & ~(pow - 1));
}

} // namespace AnyFX
//------------------------------------------------------------------------------