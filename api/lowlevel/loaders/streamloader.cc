//------------------------------------------------------------------------------
//  streamloader.cc
//  (C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "streamloader.h"
#include "shadereffect.h"
#include "base/programbase.h"
#include "base/varblockbase.h"
#include "base/shaderbase.h"
#include "base/renderstatebase.h"
#include "base/variablebase.h"
#include "base/samplerbase.h"
#include "base/varbufferbase.h"
#include "base/subroutinebase.h"
#include <assert.h>
#include <string.h>


namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
StreamLoader::StreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
StreamLoader::~StreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
	If we create our internal and external variables as an array, we should get a better cache hit.
*/
ShaderEffect* 
StreamLoader::Load()
{
	assert(this->reader->IsOpen());

	// read magic integer
	int magic = this->reader->ReadInt();

	if (magic == 'AFX1')
	{
		// load header, this must always come first!
		magic = this->reader->ReadInt();
		assert(magic == 'HEAD');
		int profile = this->reader->ReadInt();
		int major = this->reader->ReadInt();
		int minor = this->reader->ReadInt();

		// create new effect
		ShaderEffect* effect = new ShaderEffect;
		effect->header = (Implementation)profile;
		effect->major = major;
		effect->minor = minor;

		// while we are not at the end of the file, read stuff
		while (true)
		{			
			// get fourcc code for object
			int fourcc = this->reader->ReadInt();

			// check what to load
			if (this->reader->Eof())
			{
				// we're done, return
				break;
			}
			else if (fourcc == 'SHAD')
			{
				// get number of shaders and pre-allocate size
				unsigned numShaders = this->reader->ReadUInt();
				if (numShaders > 0)
				{

					unsigned i;
					for (i = 0; i < numShaders; i++)
					{
						// load shader
						ShaderBase* shader = this->shaderLoader.Load(reader, effect);
						assert(effect->shaders.find(shader->name) == effect->shaders.end());
						effect->shaders[shader->name] = shader;
					}	
				}						
			}
            else if (fourcc == 'SUBR')
            {
                unsigned numSubroutines = this->reader->ReadUInt();

				if (numSubroutines > 0)
				{
					unsigned i;
					for (i = 0; i < numSubroutines; i++)
					{
						SubroutineBase* subroutine = this->subroutineLoader.Load(reader, effect);
						assert(effect->subroutines.find(subroutine->name) == effect->subroutines.end());
						effect->subroutines[subroutine->name] = subroutine;
					}
				}
            }
			else if (fourcc == 'PROG')
			{
				// read number of programs and pre-allocate size
				unsigned numProgs = this->reader->ReadUInt();

				if (numProgs > 0)
				{
					unsigned i;
					for (i = 0; i < numProgs; i++)
					{
						// load program
						ProgramBase* program = this->programLoader.Load(reader, effect);
						assert(effect->programs.find(program->name) == effect->programs.end());
						effect->programs[program->name] = program;
					}
				}							
			}
			else if (fourcc == 'RENS')
			{
				unsigned numStates = this->reader->ReadUInt();
				if (numStates > 0)
				{
					unsigned i;
					for (i = 0; i < numStates; i++)
					{
						// load render state
						RenderStateBase* renderState = this->renderStateLoader.Load(reader, effect);
						assert(effect->renderstates.find(renderState->name) == effect->renderstates.end());
						effect->renderstates[renderState->name] = renderState;
					}
				}				
			}
			else if (fourcc == 'VARI')
			{
				unsigned numVars = this->reader->ReadUInt();
				if (numVars > 0)
				{
					unsigned i;
					for (i = 0; i < numVars; i++)
					{
						// load variable
						VariableBase* var = this->variableLoader.Load(reader, effect);
						assert(effect->variables.find(var->name) == effect->variables.end());
						effect->variables[var->name] = var;
					}
				}				
			}
			else if (fourcc == 'SAMP')
			{
				unsigned numSamplers = this->reader->ReadUInt();

				if (numSamplers > 0)
				{
					unsigned i;
					for (i = 0; i < numSamplers; i++)
					{
						// load sampler
						SamplerBase* sampler = this->samplerLoader.Load(reader, effect);
						assert(effect->samplers.find(sampler->name) == effect->samplers.end());
						effect->samplers[sampler->name] = sampler;
					}
				}								
			}
			else if (fourcc == 'VARB')
			{
				unsigned numBlocks = this->reader->ReadUInt();
				if (numBlocks > 0)
				{
					// load varblock, also 'extract' the variables defined within
					eastl::vector<VariableBase*> vars;

					unsigned i, j;
					for (i = 0; i < numBlocks; i++)
					{
						VarblockBase* varblock = this->varblockLoader.Load(reader, effect, vars);
						assert(effect->varblocks.find(varblock->name) == effect->varblocks.end());
						effect->varblocks[varblock->name] = varblock;

						// copy old variables to newly resized array
						size_t curNumVars = effect->GetNumVariables();

						// feed new array with new variables
						for (j = 0; j < vars.size(); j++)
						{
							assert(effect->variables.find(vars[j]->name) == effect->variables.end());
							effect->variables[vars[j]->name] = vars[j];
						}

						vars.clear();
					}				
				}				
			}
            else if (fourcc == 'VRBF')
            {
                unsigned numBuffers = this->reader->ReadUInt();
				if (numBuffers > 0)
				{
					unsigned i;
					for (i = 0; i < numBuffers; i++)
					{
						VarbufferBase* buffer = this->varbufferLoader.Load(reader, effect);
						assert(effect->varbuffers.find(buffer->name) == effect->varbuffers.end());
						effect->varbuffers[buffer->name] = buffer;
					}
				}
            }
			else
			{
				// unknown FourCC found, so terminate parsing, delete effect and return NULL pointer
				printf("Unknown FourCC code %d in file, suspected file corruption.\n", fourcc);
				delete effect;
				return NULL;
			}
		}

		// everything went smooth, so return effect
		return effect;
	}
	else
	{
		printf("Magic number %d is not valid for AnyFX version 1, corrupt file suspected\n", magic);
		return NULL;
	}
}

} // namespace AnyFX
