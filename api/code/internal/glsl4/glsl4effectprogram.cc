//------------------------------------------------------------------------------
//  glsl4effectprogram.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "glsl4effectprogram.h"
#include "internal/internaleffectrenderstate.h"
#include "internal/internaleffectsubroutine.h"
#include "internal/internaleffectvariable.h"
#include "glsl4effectshader.h"
#include <assert.h>
#include <algorithm>

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectProgram::GLSL4EffectProgram() :
	programHandle(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectProgram::~GLSL4EffectProgram()
{
	// empty
}

//------------------------------------------------------------------------------
/**
    FIXME! 
    Make it so we can apply a program without changing the actual OpenGL program, but instead simply change the subroutines!
*/
void 
GLSL4EffectProgram::Apply()
{
	// activate GL program
	glUseProgram(this->programHandle);

    // first time, we must apply all subroutines since
    this->ApplySubroutines();

	// if we support tessellation, then set the patch vertices parameter also
	if (this->supportsTessellation)
	{
		glPatchParameteri(GL_PATCH_VERTICES, this->patchSize);
	}

	// apply internal program
	InternalEffectProgram::Apply();
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectProgram::ApplySubroutines()
{
    // apply subroutines
    if (this->numVsSubroutines > 0) glUniformSubroutinesuiv(GL_VERTEX_SHADER,           this->numVsSubroutines, this->vsSubroutineBindings);
    if (this->numHsSubroutines > 0) glUniformSubroutinesuiv(GL_TESS_CONTROL_SHADER,     this->numHsSubroutines, this->hsSubroutineBindings);
    if (this->numDsSubroutines > 0) glUniformSubroutinesuiv(GL_TESS_EVALUATION_SHADER,  this->numDsSubroutines, this->dsSubroutineBindings);
    if (this->numGsSubroutines > 0) glUniformSubroutinesuiv(GL_GEOMETRY_SHADER,         this->numGsSubroutines, this->gsSubroutineBindings);
    if (this->numPsSubroutines > 0) glUniformSubroutinesuiv(GL_FRAGMENT_SHADER,         this->numPsSubroutines, this->psSubroutineBindings);
    if (this->numCsSubroutines > 0) glUniformSubroutinesuiv(GL_COMPUTE_SHADER,          this->numCsSubroutines, this->csSubroutineBindings);
}

//------------------------------------------------------------------------------
/**
*/
bool 
GLSL4EffectProgram::Link()
{
	// create program
	this->programHandle = glCreateProgram();

	bool needsLinking = false;

	// attach shaders
	if (0 != this->shaderBlock.vs)
	{
		GLSL4EffectShader* glsl4Shader = static_cast<GLSL4EffectShader*>(this->shaderBlock.vs);
		glAttachShader(this->programHandle, glsl4Shader->GetShaderHandle());
		needsLinking = true;
	}

	if (0 != this->shaderBlock.hs)
	{
		GLSL4EffectShader* glsl4Shader = static_cast<GLSL4EffectShader*>(this->shaderBlock.hs);
		glAttachShader(this->programHandle, glsl4Shader->GetShaderHandle());
		needsLinking = true;
	}

	if (0 != this->shaderBlock.ds)
	{
		GLSL4EffectShader* glsl4Shader = static_cast<GLSL4EffectShader*>(this->shaderBlock.ds);
		glAttachShader(this->programHandle, glsl4Shader->GetShaderHandle());
		needsLinking = true;
	}

	if (0 != this->shaderBlock.gs)
	{
		GLSL4EffectShader* glsl4Shader = dynamic_cast<GLSL4EffectShader*>(this->shaderBlock.gs);
		glAttachShader(this->programHandle, glsl4Shader->GetShaderHandle());
		needsLinking = true;
	}

	if (0 != this->shaderBlock.ps)
	{
		GLSL4EffectShader* glsl4Shader = static_cast<GLSL4EffectShader*>(this->shaderBlock.ps);
		glAttachShader(this->programHandle, glsl4Shader->GetShaderHandle());
		needsLinking = true;
	}

	if (0 != this->shaderBlock.cs)
	{
		GLSL4EffectShader* glsl4Shader = static_cast<GLSL4EffectShader*>(this->shaderBlock.cs);
		glAttachShader(this->programHandle, glsl4Shader->GetShaderHandle());
		needsLinking = true;
	}

	if (needsLinking)
	{
		// link program
		glLinkProgram(this->programHandle);

		GLint status;
		glGetProgramiv(this->programHandle, GL_LINK_STATUS, &status);

		if (status != GL_TRUE)
		{
			GLchar errorLog[65535];
			GLsizei length;
			glGetProgramInfoLog(this->programHandle, 65535, &length, errorLog);

			// create error string
			this->error = std::string(errorLog, length);

			// output false
			return false;
		}
	}
	else
	{
		// set to default program
		glDeleteProgram(this->programHandle);
		this->programHandle = 0;

        // return false since the program is empty
        return false;
	}

	return true;
}
//------------------------------------------------------------------------------
/**
    sort hook for subroutine uniforms
*/
bool
SubroutineBindingCompare(const std::pair<GLuint, GLuint>& v1, const std::pair<GLuint, GLuint>& v2)
{
	return v1.first < v2.first;
}

//------------------------------------------------------------------------------
/**
    Hmm, perhaps move this to the compile phase?
*/
void 
GLSL4EffectProgram::SetupSubroutines()
{
    assert(0 != this->programHandle);
    this->SetupSubroutineHelper(GL_VERTEX_SHADER,           this->numVsSubroutines, &this->vsSubroutineBindings, this->shaderBlock.vsSubroutines);
    this->SetupSubroutineHelper(GL_TESS_CONTROL_SHADER,     this->numHsSubroutines, &this->hsSubroutineBindings, this->shaderBlock.hsSubroutines);
    this->SetupSubroutineHelper(GL_TESS_EVALUATION_SHADER,  this->numDsSubroutines, &this->dsSubroutineBindings, this->shaderBlock.dsSubroutines);
    this->SetupSubroutineHelper(GL_GEOMETRY_SHADER,         this->numGsSubroutines, &this->gsSubroutineBindings, this->shaderBlock.gsSubroutines);
    this->SetupSubroutineHelper(GL_FRAGMENT_SHADER,         this->numPsSubroutines, &this->psSubroutineBindings, this->shaderBlock.psSubroutines);
    this->SetupSubroutineHelper(GL_COMPUTE_SHADER,          this->numCsSubroutines, &this->csSubroutineBindings, this->shaderBlock.csSubroutines);
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectProgram::SetupSubroutineHelper( GLenum shaderType, GLuint& numBindings, GLuint** bindingArray, const std::map<std::string, InternalEffectSubroutine*>& bindings )
{
    std::map<std::string, InternalEffectSubroutine*>::const_iterator it;

    std::vector<std::pair<GLuint, GLuint> > intermediateMap;
    unsigned numActiveSubroutines = 0;
    for (it = bindings.begin(); it != bindings.end(); it++)
    {
        std::string var = (*it).first;
        InternalEffectSubroutine* imp = (*it).second;

        GLint subroutineIndex = glGetSubroutineIndex(this->programHandle, shaderType, imp->GetName().c_str());
        GLint uniformIndex = glGetSubroutineUniformLocation(this->programHandle, shaderType, var.c_str());

        if (uniformIndex != -1)
        {
            intermediateMap.push_back(std::pair<GLuint, GLuint>(uniformIndex, subroutineIndex));
            numActiveSubroutines++;
        }
    }

    (*bindingArray) = new GLuint[numActiveSubroutines];
    std::sort(intermediateMap.begin(), intermediateMap.end(), SubroutineBindingCompare);
    unsigned i;
    for (i = 0; i < numActiveSubroutines; i++)
    {
        (*bindingArray)[i] = intermediateMap[i].second;
    }

    numBindings = numActiveSubroutines;
}
} // namespace AnyFX
