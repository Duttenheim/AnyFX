//------------------------------------------------------------------------------
//  glsl4effectshader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "glsl4effectshader.h"
#include <assert.h>
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectShader::GLSL4EffectShader() :
	shaderHandle(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectShader::~GLSL4EffectShader()
{
	glDeleteShader(this->shaderHandle);
}

//------------------------------------------------------------------------------
/**
*/
bool 
GLSL4EffectShader::Compile()
{
	assert(this->sourceCode.size() > 0);

	// create shader object based on type
	switch (this->type)
	{
	case VertexShader:
		this->shaderHandle = glCreateShader(GL_VERTEX_SHADER);
		break;
	case PixelShader:
		this->shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case HullShader:
		this->shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case DomainShader:
		this->shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	case GeometryShader:
		this->shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case ComputeShader:
		this->shaderHandle = glCreateShader(GL_COMPUTE_SHADER);
		break;
	}
	assert(this->shaderHandle != 0);

	const GLchar* source = this->sourceCode.c_str();
	GLint length = this->sourceCode.length();

	// set source for shader
	glShaderSource(this->shaderHandle, 1, (const GLchar**)&source, &length);

	// now finally compile
	glCompileShader(this->shaderHandle);

	// if there is an error, report it, although the compiler should already have done this
	GLint status;
	glGetShaderiv(this->shaderHandle, GL_COMPILE_STATUS, &status);
    GLint errorSize;
    glGetShaderiv(this->shaderHandle, GL_INFO_LOG_LENGTH, &errorSize);

	if (status != GL_TRUE)
	{
        GLchar* errorLog = new GLchar[errorSize];
		glGetShaderInfoLog(this->shaderHandle, errorSize, NULL, errorLog);

		// create error string
		this->error = std::string(errorLog, errorSize);
        delete [] errorLog;

		// output error
		return false;
	}

	// everything is fine
	return true;
}
} // namespace AnyFX