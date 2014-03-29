#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::GLSL4EffectProgram
    
    GLSL4 backend for EffectProgram, implements InternalEffectProgram.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "internal/internaleffectprogram.h"
#include "GL/glew.h"
namespace AnyFX
{
class GLSL4EffectProgram : public InternalEffectProgram
{
public:
	/// constructor
	GLSL4EffectProgram();
	/// destructor
	virtual ~GLSL4EffectProgram();

private:
	friend class GLSL4EffectVariable;
	friend class GLSL4EffectVarblock;

	/// applies program
	void Apply();
    /// applies subroutines
    void ApplySubroutines();
	/// links program
	bool Link();

    /// sets up subroutine mappings
    void SetupSubroutines();
    /// helper function for subroutine setup
    void SetupSubroutineHelper(GLenum shaderType, GLuint& numBindings, GLuint** bindingArray, const std::map<std::string, InternalEffectSubroutine*>& bindings);

	GLuint programHandle;

    GLuint* vsSubroutineBindings;
    GLuint numVsSubroutines;
    GLuint* hsSubroutineBindings;
    GLuint numHsSubroutines;
    GLuint* dsSubroutineBindings;
    GLuint numDsSubroutines;
    GLuint* gsSubroutineBindings;
    GLuint numGsSubroutines;
    GLuint* psSubroutineBindings;
    GLuint numPsSubroutines;
    GLuint* csSubroutineBindings;
    GLuint numCsSubroutines;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------