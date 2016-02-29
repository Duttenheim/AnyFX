//------------------------------------------------------------------------------
//	AnyFX generator functions for HLSL based code generation
//	(C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------

#include <string>
#include "shader.h"
#include "generator.h"
#include "programrow.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
	Generate HLSL code from shader
*/
std::string
Shader::GenerateHLSL(AnyFX::Generator* generator, int major, int minor)
{
	switch (major)
	{
		case 4: return GenerateHLSL5(generator);
		case 3: return GenerateHLSL4(generator);
	}
	return "";
}

} // namespace AnyFX