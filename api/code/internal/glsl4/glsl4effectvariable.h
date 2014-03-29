#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::GLSL4EffectVariable
    
    GLSL4 backend for EffectVariable, implements InternalEffectVariable.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectvariable.h"
#include "GL/glew.h"
#include <map>
namespace AnyFX
{
class InternalEffectProgram;
class GLSL4EffectVariable : public InternalEffectVariable
{
public:
	/// constructor
	GLSL4EffectVariable();
	/// destructor
	virtual ~GLSL4EffectVariable();

private:
	friend class GLSL4EffectSampler;

	/// sets up variable from program, override in subclass
	void Setup(std::vector<InternalEffectProgram*> programs, const std::string& defaultValue);
	/// sets up texture-specific stuff
	void MakeTexture();
	/// activates variable, this makes the uniform location be the one found in the given program
	void Activate(InternalEffectProgram* program);

	/// apply variable, only applies to textures
	void Apply();
	/// commits variable to glsl shader
	void Commit();

	std::map<GLint, GLint> uniformProgramMap;
	GLuint activeProgram;
	GLint uniformLocation;
	GLint textureUnit;
	GLenum textureType;
	GLenum glAccessMode;
	GLenum glImageFormat;

	
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------