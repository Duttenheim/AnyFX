#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::GLSL4EffectVarblock
    
    GLSL4 backend for EffectVarblock, implements InternalEffectVarblock.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectvarblock.h"
#include "GL/glew.h"
#include <map>
namespace AnyFX
{
class GLSL4EffectVarblock : public InternalEffectVarblock
{
public:
	/// constructor
	GLSL4EffectVarblock();
	/// destructor
	virtual ~GLSL4EffectVarblock();

protected:

	static const GLuint MaxBackingBuffers = 4;

	/// sets up variable block from program
	void Setup(std::vector<InternalEffectProgram*> programs);
	/// sets up varblock from programs using a pre-existing varblock
	void SetupSlave(std::vector<InternalEffectProgram*> programs, InternalEffectVarblock* master);

	/// binds varblock
	void Apply();
	/// updates variable block
	void Commit();
	/// activates varblock uniform location
	void Activate(InternalEffectProgram* program);

	std::map<GLint, GLint> uniformBlockProgramMap;
	GLuint activeProgram;
	GLuint uniformBlockBinding;
	GLuint uniformBlockLocation;

	GLuint buffer;
}; 


} // namespace AnyFX
//------------------------------------------------------------------------------