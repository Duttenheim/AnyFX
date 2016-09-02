
#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::Shader
    
	A shader represents a composed target-language representation of a function.

	Its main responsibilities is to format a given function, 
	variable list and external functions, 
	into something a target language shader compiler can understand.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include "function.h"
#include "variable.h"
#include "header.h"
#include "glslang/Public/ShaderLang.h"


namespace AnyFX
{
class Variable;
class Structure;
class VarBlock;
class Function;
class Constant;
class VarBuffer;
class Subroutine;
class Sampler;
class Shader : public Compileable
{
public:
	
	/// constructor
	Shader();
	/// destructor
	virtual ~Shader();

	/// sets the shader type
	void SetType(const unsigned type);
	/// gets the shader type
	unsigned GetType() const;
	/// sets the function object which this shader will use as its main entry point
	void SetFunction(const Function& func);
	/// gets the function
	const Function& GetFunction() const;
	/// sets shader name
	void SetName(const std::string& name);
	/// gets shader name
	const std::string& GetName() const;

	/// returns formatted code, is only viable after compilation is done
	const std::string& GetCompiledCode() const;

	/// set compile flags for this shader
	void SetCompileFlags(const std::string& flags);

	/// set subroutine mappings
	void SetSubroutineMappings(const std::map<std::string, std::string>& subroutineMappings);

	/// type check shader
	void TypeCheck(TypeChecker& typechecker);
	/// compile shader
	void Compile(BinWriter& writer);

	/// sets up shader
	void Setup();
	/// generates code for shader
	void Generate(
		Generator& generator,
		const std::vector<Variable>& vars,
		const std::vector<Structure>& structures,
		const std::vector<Constant>& constants,
		const std::vector<VarBlock>& blocks, 
        const std::vector<VarBuffer>& buffers,
		const std::vector<Sampler>& samplers,
        const std::vector<Subroutine>& subroutines,
		const std::vector<Function>& functions,
		const std::vector<std::string>& passthroughPPs);

	/// static function which resets all bindings
	static void ResetBindings();

	// each binding index is a slot for a descriptor set in Vulkan, or is statically assigned in other languages
	/*
		0: varblocks
		1: varbuffers
		2: samplers (texture-sampler combos)
		3: images (read-write textures)
		4: sampler objects
	*/
	static unsigned bindingIndices[64]; 
private:
	friend class Program;

#pragma region GLSL
	std::string GenerateGLSL(AnyFX::Generator* generator, int major, int minor);

	// sub generation functions
	std::string GenerateGLSL4(AnyFX::Generator* generator);
	std::string GenerateGLSL3(AnyFX::Generator* generator);
#pragma endregion

#pragma region HLSL
	std::string GenerateHLSL(AnyFX::Generator* generator, int major, int minor);
	std::string GenerateHLSL5(AnyFX::Generator* generator);
	std::string GenerateHLSL4(AnyFX::Generator* generator);
#pragma endregion

	std::string GenerateSPIRV(AnyFX::Generator* generator, int major, int minor);
	std::string GenerateMetal(AnyFX::Generator* generator, int major, int minor);

#pragma region OpenGL
	/// compiles the generated code to check for validation in GLSL
	void CompileGLSL(const std::string& code, Generator* generator);
	/// compiles the generated code to check for validation in GLSL
	void CompileSPIRV(const std::string& code, Generator* generator);

	/// generates GLSL4 target code
	void GenerateGLSL4(Generator& generator);
	/// generates GLSL3 target code
	void GenerateGLSL3(Generator& generator);

	/// output AnyFX formatted GLSL problem using the AMD/Intel syntax
	void GLSLProblemIntelATI(Generator* generator, std::stringstream& stream);
	/// output AnyFX formatted GLSL problem using the NVIDIA syntax
	void GLSLProblemNvidia(Generator* generator, std::stringstream& stream);
	/// output AnyFX formatted GLSL problem using the Khronos syntax
	void GLSLProblemKhronos(Generator* generator, std::stringstream& stream);
#pragma endregion

#pragma region DirectX
	/// generates HLSL5 target code
	void GenerateHLSL5(Generator& generator);
	/// generates HLSL4 target code
	void GenerateHLSL4(Generator& generator);
	/// generates HLSL3 target code
	void GenerateHLSL3(Generator& generator);
#pragma endregion

	Function func;
	unsigned shaderType;
	std::string name;
	std::string formattedCode;
    std::string compileFlags;
	unsigned codeOffset;

	glslang::TShader* glslShader;
	void* hlslShader;
	char* binary;
	unsigned binarySize;
	bool binaryValid;

	std::string preamble;
    std::map<int, std::pair<std::string, std::string> > indexToFileMap;
	std::map<std::string, std::string> subroutineMappings;

}; 

//------------------------------------------------------------------------------
/**
*/
inline void
Shader::SetFunction(const Function& func)
{
	this->func = func;
}

//------------------------------------------------------------------------------
/**
*/
inline const Function& 
Shader::GetFunction() const
{
	return this->func;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Shader::SetName( const std::string& name )
{
	this->name = name;
    this->func.SetName(name);
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
Shader::GetName() const
{
	return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string&
Shader::GetCompiledCode() const
{
	return this->formattedCode;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Shader::SetType( const unsigned type )
{
	this->shaderType = type;
}

//------------------------------------------------------------------------------
/**
*/
inline unsigned 
Shader::GetType() const
{
	return this->shaderType;
}
//------------------------------------------------------------------------------
/**
*/
inline void
Shader::SetCompileFlags(const std::string& flags)
{
    this->compileFlags = flags;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Shader::SetSubroutineMappings(const std::map<std::string, std::string>& subroutineMappings)
{
	this->subroutineMappings = subroutineMappings;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Shader::ResetBindings()
{
	memset(Shader::bindingIndices, 0, sizeof(Shader::bindingIndices));
}

} // namespace AnyFX
//------------------------------------------------------------------------------
