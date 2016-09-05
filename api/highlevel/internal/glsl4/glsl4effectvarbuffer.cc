//------------------------------------------------------------------------------
//  glsl4effectvarbuffer.cc
//  (C) 2014 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "glsl4effectvarbuffer.h"
#include "glsl4effectprogram.h"
#include "glsl4effectvariable.h"
#include <assert.h>
#include "effectvarbuffer.h"

#define a_max(x, y) (x > y ? x : y)
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectVarbuffer::GLSL4EffectVarbuffer() :
	shaderStorageBlockBinding(GL_INVALID_INDEX)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectVarbuffer::~GLSL4EffectVarbuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
GLSL4EffectVarbuffer::Setup(eastl::vector<InternalEffectProgram*> programs)
{
	InternalEffectVarbuffer::Setup(programs);

	glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &this->offsetAlignment);

	unsigned i;
	for (i = 0; i < programs.size(); i++)
	{
		GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(programs[i]);
		assert(0 != opengl4Program);

		GLuint location = glGetProgramResourceIndex(opengl4Program->programHandle, GL_SHADER_STORAGE_BLOCK, this->name.c_str());
		this->activeMap[opengl4Program] = location;
		if (location != GL_INVALID_INDEX)
		{
			GLenum prop = GL_BUFFER_BINDING;
			glGetProgramResourceiv(opengl4Program->programHandle, GL_SHADER_STORAGE_BLOCK, location, 1, &prop, 1, NULL, (GLint*)&this->shaderStorageBlockBinding);
			//glShaderStorageBlockBinding(opengl4Program->programHandle, location, this->shaderStorageBlockBinding);
			this->active = true;
		}
#if GL4_MULTIBIND
		opengl4Program->glsl4Varbuffers.push_back(this);
		opengl4Program->varbufferBindsCount = a_max(this->shaderStorageBlockBinding + 1, opengl4Program->varbufferBindsCount);
#endif
	}
}

//------------------------------------------------------------------------------
/**
*/
void
GLSL4EffectVarbuffer::SetupSlave(eastl::vector<InternalEffectProgram*> programs, InternalEffectVarbuffer* master)
{
	InternalEffectVarbuffer::SetupSlave(programs, master);

	GLSL4EffectVarbuffer* mainBuffer = dynamic_cast<GLSL4EffectVarbuffer*>(master);
	assert(0 != mainBuffer);

	//this->shaderStorageBlockBinding = mainBuffer->shaderStorageBlockBinding;
	glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &this->offsetAlignment);

	unsigned i;
	for (i = 0; i < programs.size(); i++)
	{
		GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(programs[i]);
		assert(0 != opengl4Program);

		GLuint location = glGetProgramResourceIndex(opengl4Program->programHandle, GL_SHADER_STORAGE_BLOCK, this->name.c_str());
		this->activeMap[opengl4Program] = location;
		if (location != GL_INVALID_INDEX)
		{
			GLenum prop = GL_BUFFER_BINDING;
			glGetProgramResourceiv(opengl4Program->programHandle, GL_SHADER_STORAGE_BLOCK, location, 1, &prop, 1, NULL, (GLint*)&this->shaderStorageBlockBinding);
			//glShaderStorageBlockBinding(opengl4Program->programHandle, location, this->shaderStorageBlockBinding);
			mainBuffer->active = true;
		}
#if GL4_MULTIBIND
		opengl4Program->glsl4Varbuffers.push_back(this);
		opengl4Program->varbufferBindsCount = a_max(this->shaderStorageBlockBinding + 1, opengl4Program->varbufferBindsCount);
#endif
	}
}

//------------------------------------------------------------------------------
/**
	Eh, can we even base bind 
*/
void
GLSL4EffectVarbuffer::Commit()
{
	if (this->currentLocation != GL_INVALID_INDEX && this->shaderStorageBlockBinding != GL_INVALID_INDEX)
	{
		OpenGLBufferBinding* buf = (OpenGLBufferBinding*)*this->bufferHandle;
		if (buf != 0)
		{
			if (buf->bindRange)
			{
#if GL4_MULTIBIND
				GLSL4EffectProgram::SetVarbufferBinding(this->shaderStorageBlockBinding, this->masterBuffer->isDirty, buf->handle, buf->offset, buf->size);
				/*
				this->activeProgram->varbufferRangeBindBuffers[this->shaderStorageBlockBinding] = buf->handle;
				this->activeProgram->varbufferRangeBindOffsets[this->shaderStorageBlockBinding] = buf->offset;
				this->activeProgram->varbufferRangeBindSizes[this->shaderStorageBlockBinding] = buf->size;
				this->activeProgram->varbuffersDirty = true;
				*/
#else
				GLSL4VarbufferRangeState state;
				state.buffer = buf->handle;
				state.offset = buf->offset;
				state.length = buf->size;
				GLuint binding = this->shaderStorageBlockBinding;
				if (GLSL4VarbufferRangeStates[binding] != state)
				{
					//if (state.length % this->offsetAlignment != 0) printf("WOW, THIS OFFSET IS NOT GOOD! %d\n", state.length);
					//assert(state.length % this->offsetAlignment == 0);
					GLSL4VarbufferRangeStates[binding] = state;
					glBindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, state.buffer, state.offset, state.length);
				}
#endif
			}
			else
			{
#if GL4_MULTIBIND
				GLSL4EffectProgram::SetVarbufferBinding(this->shaderStorageBlockBinding, this->masterBuffer->isDirty, buf->handle, 0, buf->size);
				/*
				this->activeProgram->varbufferRangeBindBuffers[this->shaderStorageBlockBinding] = buf->handle;
				this->activeProgram->varbufferRangeBindOffsets[this->shaderStorageBlockBinding] = 0;
				this->activeProgram->varbufferRangeBindSizes[this->shaderStorageBlockBinding] = buf->size;
				this->activeProgram->varbuffersDirty = true;
				*/
#else
				GLSL4VarbufferBaseState state;
				state.buffer = buf->handle;
				GLuint binding = this->shaderStorageBlockBinding;
				if (GLSL4VarbufferBaseStates[binding] != state)
				{
					GLSL4VarbufferBaseStates[binding] = state;
					glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, state.buffer);
				}
#endif
			}
		}
		else
		{
			GLSL4EffectProgram::SetVarbufferBinding(this->shaderStorageBlockBinding, this->masterBuffer->isDirty, 0, 0, 1);
			/*
			this->activeProgram->varblockRangeBindBuffers[this->shaderStorageBlockBinding] = 0;
			this->activeProgram->varblockRangeBindOffsets[this->shaderStorageBlockBinding] = 0;
			this->activeProgram->varblockRangeBindSizes[this->shaderStorageBlockBinding] = 1;
			this->activeProgram->varbuffersDirty = true;
			*/
		}
	}

	// uncheck dirty flag
	this->masterBuffer->isDirty = false;
}

//------------------------------------------------------------------------------
/**
*/
void
GLSL4EffectVarbuffer::Activate(InternalEffectProgram* program)
{
	GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(program);
	assert(0 != opengl4Program);
	this->activeProgram = opengl4Program;
	this->activeProgramHandle = opengl4Program->programHandle;
	this->currentLocation = this->activeMap[opengl4Program];
}

} // namespace AnyFX
