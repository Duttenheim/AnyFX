//------------------------------------------------------------------------------
//  glsl4effectvariable.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "glsl4effectvariable.h"
#include "glsl4effectprogram.h"
#include <assert.h>
#include "internal/internaleffectvarblock.h"


#if __ANYFX_TRANSPOSE_MATRIX__
	#define __MATRIX_TRANSPOSE__ GL_TRUE
#else
	#define __MATRIX_TRANSPOSE__ GL_FALSE
#endif

static const GLenum opengl4AccessTable[] = 
{
	GL_READ_ONLY,
	GL_WRITE_ONLY,
	GL_READ_WRITE
};

static const GLenum opengl4ImageFormat[] =
{
	GL_RGBA32F,
	GL_RGBA16F,
	GL_RG32F,
	GL_RG16F,
	GL_R11F_G11F_B10F,
	GL_R32F,
	GL_R16F,
	GL_RGBA16,
	GL_RGB10_A2,
	GL_RG16,
	GL_RG8,
	GL_R8,
	GL_RGBA16_SNORM,
	GL_RGBA8_SNORM,
	GL_RG16_SNORM,
	GL_RG8_SNORM,
	GL_R16_SNORM,
	GL_R8_SNORM,
	GL_RGBA32I,
	GL_RGBA16I,
	GL_RGBA8I,
	GL_RG32I,
	GL_RG16I,
	GL_RG8I,
	GL_R32I,
	GL_R16I,
	GL_R8I,
	GL_RGBA32UI,
	GL_RGBA16UI,
	GL_RGBA8UI,
	GL_RG32UI,
	GL_RG16UI,
	GL_RG8UI,
	GL_R32UI,
	GL_R16UI,
	GL_R8UI
};

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectVariable::GLSL4EffectVariable() :
	uniformLocation(-1),
	activeProgram(0),
	glAccessMode(GL_NONE),
	glImageFormat(GL_NONE),
	textureUnit(GL_TEXTURE0),
	textureType(GL_TEXTURE_2D)
{
	this->uniformProgramMap.clear();
}

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectVariable::~GLSL4EffectVariable()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVariable::Setup( std::vector<InternalEffectProgram*> programs, const std::string& defaultValue )
{
	InternalEffectVariable::Setup(programs, defaultValue);

	if (!this->isInVarblock)
	{
		unsigned i;
		for (i = 0; i < programs.size(); i++)
		{
			GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(programs[i]);
			assert(0 != opengl4Program);

			GLint location = glGetUniformLocation(opengl4Program->programHandle, this->name.c_str());
			this->uniformProgramMap[opengl4Program->programHandle] = location;

			if (location != -1)
			{
				this->active = true;
			}
		}
	}
	else
	{
		// unfortunately, we cannot know if a variable inside a variable block is used...
		this->active = true;
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVariable::MakeTexture()
{
	this->textureUnit = InternalEffectVariable::globalTextureCounter++;

	switch (this->type)
	{
	case Image1D:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case Sampler1D:
		this->textureType = GL_TEXTURE_1D;
		break;
	case Image1DArray:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case Sampler1DArray:
		this->textureType = GL_TEXTURE_1D_ARRAY;
		break;
	case Image2D:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case Sampler2D:
		this->textureType = GL_TEXTURE_2D;
		break;
	case Image2DArray:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case Sampler2DArray:
		this->textureType = GL_TEXTURE_2D_ARRAY;
		break;
	case Image2DMS:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case Sampler2DMS:
		this->textureType = GL_TEXTURE_2D_MULTISAMPLE;
		break;
	case Image2DMSArray:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case Sampler2DMSArray:
		this->textureType = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		break;
	case Image3D:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case Sampler3D:
		this->textureType = GL_TEXTURE_3D;
		break;
	case ImageCube:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case SamplerCube:
		this->textureType = GL_TEXTURE_CUBE_MAP;
		break;
	case ImageCubeArray:
		this->glAccessMode = opengl4AccessTable[this->access];
		this->glImageFormat = opengl4ImageFormat[this->format];
	case SamplerCubeArray:
		this->textureType = GL_TEXTURE_CUBE_MAP_ARRAY;
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVariable::Activate( InternalEffectProgram* program )
{
	InternalEffectVariable::Activate(program);
	GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(program);
	assert(0 != opengl4Program);
	if (!isInVarblock && this->active)
	{
		assert(this->uniformProgramMap.find(opengl4Program->programHandle) != this->uniformProgramMap.end());
		this->uniformLocation = this->uniformProgramMap[opengl4Program->programHandle];
		this->activeProgram = opengl4Program->programHandle;	
	}	
	else
	{
		this->uniformLocation = -1;
	}
}


//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVariable::Apply()
{
	if (this->type >= Sampler1D && this->type <= ImageCubeArray)
	{
		// first bind variable name to texture unit
		glUniform1i(this->uniformLocation, this->textureUnit);
	}	
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVariable::Commit()
{
    // never apply subroutine variables
    if (this->isSubroutine) return;
	if (!this->isInVarblock && this->uniformLocation != -1 && this->isDirty)
	{
		switch (this->type)
		{
		case Float:
		case Double:
			glUniform1fv(this->uniformLocation, this->commitSize, (GLfloat*)this->currentValue);
			break;
		case Float2:
		case Double2:
			glUniform2fv(this->uniformLocation, this->commitSize, (GLfloat*)this->currentValue);
			break;
		case Float3:
		case Double3:
			glUniform3fv(this->uniformLocation, this->commitSize, (GLfloat*)this->currentValue);
			break;
		case Float4:
		case Double4:
			glUniform4fv(this->uniformLocation, this->commitSize, (GLfloat*)this->currentValue);
			break;
		case Integer:
		case Short:
		case Bool:
			glUniform1iv(this->uniformLocation, this->commitSize, (GLint*)this->currentValue);
			break;
		case Integer2:
		case Short2:
		case Bool2:
			glUniform2iv(this->uniformLocation, this->commitSize, (GLint*)this->currentValue);
			break;
		case Integer3:
		case Short3:
		case Bool3:
			glUniform3iv(this->uniformLocation, this->commitSize, (GLint*)this->currentValue);
			break;
		case Integer4:
		case Short4:
		case Bool4:
			glUniform4iv(this->uniformLocation, this->commitSize, (GLint*)this->currentValue);
			break;
		case UInteger:
			glUniform1uiv(this->uniformLocation, this->commitSize, (GLuint*)this->currentValue);
			break;
		case UInteger2:
			glUniform2uiv(this->uniformLocation, this->commitSize, (GLuint*)this->currentValue);
			break;
		case UInteger3:
			glUniform3uiv(this->uniformLocation, this->commitSize, (GLuint*)this->currentValue);
			break;
		case UInteger4:
			glUniform4uiv(this->uniformLocation, this->commitSize, (GLuint*)this->currentValue);
			break;
		case Matrix2x2:
			glUniformMatrix2fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix2x3:
			glUniformMatrix2x3fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix2x4:
			glUniformMatrix2x4fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix3x2:
			glUniformMatrix3x2fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix3x3:
			glUniformMatrix3fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix3x4:
			glUniformMatrix3x4fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix4x2:
			glUniformMatrix4x2fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix4x3:
			glUniformMatrix4x3fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Matrix4x4:
			glUniformMatrix4fv(this->uniformLocation, this->commitSize, __MATRIX_TRANSPOSE__, (GLfloat*)this->currentValue);
			break;
		case Sampler1D:
		case Sampler1DArray:
		case Sampler2D:
		case Sampler2DArray:
		case Sampler2DMS:
		case Sampler2DMSArray:
		case Sampler3D:
		case SamplerCube:
		case SamplerCubeArray:
			{
				// now select this texture unit to be the active texture
				glActiveTexture(GL_TEXTURE0 + this->textureUnit);

				// unpack data
				EffectVariable::OpenGLTexture* obj = (EffectVariable::OpenGLTexture*)this->currentValue;

				// only set obj if the current value is not null
				if (obj && obj->textureType == this->textureType)
				{
					// now bind the texture to this slot
					glBindTexture(obj->textureType, obj->texture);
				}
                else
                {
                    // otherwise, bind texture to 0
                    glBindTexture(this->textureType, 0);
                }
				break;
			}
		case Image1D:
		case Image1DArray:
		case Image2D:
		case Image2DArray:
		case Image2DMS:
		case Image2DMSArray:
		case Image3D:
		case ImageCube:
		case ImageCubeArray:
			{
				// unpack data
				EffectVariable::OpenGLTexture* obj = (EffectVariable::OpenGLTexture*)this->currentValue;

                if (obj && obj->textureType == this->textureType)
                {
                    // now select this image unit to be the active image (as apparent, this is not very nice since the format and read/write is hard coded)
                    glBindImageTexture(this->textureUnit, obj->texture, 0, GL_TRUE, 0, this->glAccessMode, this->glImageFormat);
                }
                else
                {
                    glBindImageTexture(this->textureUnit, 0, 0, GL_TRUE, 0, this->glAccessMode, this->glImageFormat);
                }

				
				break;
			}
		}

		// uncheck dirty flag
		this->isDirty = false;
	}
}

} // namespace AnyFX
