//------------------------------------------------------------------------------
//  glsl4effectvarblock.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "glsl4effectvarblock.h"
#include "glsl4effectprogram.h"
#include <assert.h>
#include "internal/internaleffectvariable.h"

namespace AnyFX
{

GLuint InternalEffectVarblock::globalUniformBlockBinding = 0;
//------------------------------------------------------------------------------
/**
*/
GLSL4EffectVarblock::GLSL4EffectVarblock() :
	buffer(-1),
	activeProgram(-1),
	uniformBlockLocation(-1)
{
	this->uniformBlockProgramMap.clear();
}

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectVarblock::~GLSL4EffectVarblock()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVarblock::Setup( std::vector<InternalEffectProgram*> programs )
{
	InternalEffectVarblock::Setup(programs);

	this->uniformBlockBinding = globalUniformBlockBinding++;
	unsigned i;
	for (i = 0; i < programs.size(); i++)
	{
		GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(programs[i]);
		assert(0 != opengl4Program);

		GLint location = glGetUniformBlockIndex(opengl4Program->programHandle, this->name.c_str());
		this->uniformBlockProgramMap[opengl4Program->programHandle] = location;

		if (location != -1)
		{
			// now tell the program in which binding slot this buffer should be 
			glUniformBlockBinding(opengl4Program->programHandle, location, this->uniformBlockBinding);	
		}
	}

	// generate data buffer
	glGenBuffers(1, &this->buffer);

	// bind buffer and initialize size
	glBindBuffer(GL_UNIFORM_BUFFER, this->buffer);
	glBufferData(GL_UNIFORM_BUFFER, this->dataBlock->size, NULL, GL_STATIC_DRAW);

	// unbind last buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVarblock::SetupSlave( std::vector<InternalEffectProgram*> programs, InternalEffectVarblock* master )
{
	InternalEffectVarblock::SetupSlave(programs, master);

	this->uniformBlockBinding = globalUniformBlockBinding++;
	unsigned i;
	for (i = 0; i < programs.size(); i++)
	{
		GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(programs[i]);
		assert(0 != opengl4Program);

		GLint location = glGetUniformBlockIndex(opengl4Program->programHandle, this->name.c_str());
		this->uniformBlockProgramMap[opengl4Program->programHandle] = location;

		if (location != -1)
		{
			this->active = true;

			// now tell the program in which binding slot this buffer should be 
			glUniformBlockBinding(opengl4Program->programHandle, location, this->uniformBlockBinding);
		}
	}

	// assert the master block is of same backend
	GLSL4EffectVarblock* opengl4Varblock = dynamic_cast<GLSL4EffectVarblock*>(master);
	assert(0 != opengl4Varblock);

	// copy GL buffer
	this->buffer = opengl4Varblock->buffer;
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVarblock::Apply()
{
	// bind buffer to binding point, piece of cake!
	glBindBufferBase(GL_UNIFORM_BUFFER, this->uniformBlockBinding, this->buffer);
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVarblock::Commit()
{
	// spare some performance by only updating the block if it's dirty
	if (this->isDirty)
	{
		// bind buffer, orphan the old one, and buffer new data
		glBindBuffer(GL_UNIFORM_BUFFER, this->buffer);
		glInvalidateBufferData(this->buffer);
		glBufferData(GL_UNIFORM_BUFFER, this->dataBlock->size, this->dataBlock->data, GL_STATIC_DRAW);

		// reset dirty flag
		this->isDirty = false;

		// also make sure the master block is flagged as not dirty anymore
		if (this->masterBlock)
		{
			this->masterBlock->isDirty = false;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectVarblock::Activate( InternalEffectProgram* program )
{
	GLSL4EffectProgram* opengl4Program = dynamic_cast<GLSL4EffectProgram*>(program);
	assert(0 != opengl4Program);
	assert(this->uniformBlockProgramMap.find(opengl4Program->programHandle) != this->uniformBlockProgramMap.end());
	this->uniformBlockLocation = this->uniformBlockProgramMap[opengl4Program->programHandle];
	this->activeProgram = opengl4Program->programHandle;
}

} // namespace AnyFX