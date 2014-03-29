//------------------------------------------------------------------------------
//  effectstreamloader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectstreamloader.h"
#include "effect.h"
#include "effectprogram.h"
#include "effectvarblock.h"
#include "effectshader.h"
#include "effectrenderstate.h"
#include "effectvariable.h"
#include "effectsampler.h"
#include "effectvarbuffer.h"
#include "effectsubroutine.h"
#include <assert.h>
#include <string.h>
#include "internal/internaleffectvariable.h"
#include "internal/internaleffectvarblock.h"


namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectStreamLoader::EffectStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectStreamLoader::~EffectStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
Effect* 
EffectStreamLoader::Load()
{
	assert(this->reader->IsOpen());

	// read magic integer
	int magic = this->reader->ReadInt();

	if (magic == 'AFX1')
	{
		// load header, this must always come first!
		magic = this->reader->ReadInt();
		assert(magic == 'HEAD');
		Effect::EffectType profile = (Effect::EffectType)this->reader->ReadInt();
		int major = this->reader->ReadInt();
		int minor = this->reader->ReadInt();

		// create new effect
		Effect* effect = new Effect;
		effect->type = profile;
		effect->major = major;
		effect->minor = minor;

		// reset global texture counters
		// reset the global texture counter for texture units, and global uniform block counter for buffer units
		InternalEffectVariable::globalTextureCounter = 0;
		InternalEffectVarblock::globalUniformBlockBinding = 0;

		// while we are not at the end of the file, read stuff
		while (true)
		{
			// get fourcc code for object
			int fourcc = this->reader->ReadInt();

			// check what to load
			if (fourcc == 'SHAD')
			{
				// get number of shaders and pre-allocate size
				unsigned numShaders = this->reader->ReadUInt();
				effect->shadersByIndex = new EffectShader*[numShaders];
				effect->numShaders = numShaders;

				unsigned i;
				for (i = 0; i < numShaders; i++)
				{
					// load shader
					EffectShader* shader = this->shaderLoader.Load(reader, effect);

					assert(effect->shadersByName.find(shader->GetName()) == effect->shadersByName.end());
					effect->shadersByIndex[i] = shader;
					effect->shadersByName[shader->GetName()] = shader;
				}				
			}
            else if (fourcc == 'SUBR')
            {
                unsigned numSubroutines = this->reader->ReadUInt();
                effect->subroutinesByIndex = new EffectSubroutine*[numSubroutines];
                effect->numSubroutines = numSubroutines;

                unsigned i;
                for (i = 0; i < numSubroutines; i++)
                {
                    EffectSubroutine* subroutine = this->subroutineLoader.Load(reader, effect);
                    assert(effect->subroutinesByName.find(subroutine->GetName()) == effect->subroutinesByName.end());
                    effect->subroutinesByIndex[i] = subroutine;
                    effect->subroutinesByName[subroutine->GetName()] = subroutine;
                }
            }
			else if (fourcc == 'PROG')
			{
				// read number of programs and pre-allocate size
				unsigned numProgs = this->reader->ReadUInt();
				effect->programsByIndex = new EffectProgram*[numProgs];
				effect->numPrograms = numProgs;

				unsigned i;
				for (i = 0; i < numProgs; i++)
				{
					// load program
					EffectProgram* program = this->programLoader.Load(reader, effect);

					// only add program if the loading was successful, this also includes the linking process
					assert(effect->programsByName.find(program->GetName()) == effect->programsByName.end());
					effect->programsByIndex[i] = program;
					effect->programsByName[program->GetName()] = program;
				}			
			}
			else if (fourcc == 'RENS')
			{
				unsigned numStates = this->reader->ReadUInt();
				effect->renderStatesByIndex = new EffectRenderState*[numStates];
				effect->numRenderStates = numStates;

				unsigned i;
				for (i = 0; i < numStates; i++)
				{
					// load render state
					EffectRenderState* renderState = this->renderStateLoader.Load(reader, effect);
					assert(effect->renderStatesByName.find(renderState->GetName()) == effect->renderStatesByName.end());
					effect->renderStatesByIndex[i] = renderState;
					effect->renderStatesByName[renderState->GetName()] = renderState;
				}				
			}
			else if (fourcc == 'VARI')
			{
				unsigned numVars = this->reader->ReadUInt();
				effect->numVariables = numVars;
				effect->variablesByIndex = new EffectVariable*[numVars];

				unsigned i;
				for (i = 0; i < numVars; i++)
				{
					// load variable
					EffectVariable* var = this->variableLoader.Load(reader, effect);
					assert(effect->variablesByName.find(var->GetName()) == effect->variablesByName.end());
					
					effect->variablesByIndex[i] = var;
					effect->variablesByName[var->GetName()] = var;
				}				
			}
			else if (fourcc == 'SAMP')
			{
				unsigned numSamplers = this->reader->ReadUInt();
				effect->samplersByIndex = new EffectSampler*[numSamplers];
				effect->numSamplers = numSamplers;

				unsigned i;
				for (i = 0; i < numSamplers; i++)
				{
					// load sampler
					EffectSampler* sampler = this->samplerLoader.Load(reader, effect);
					assert(effect->samplersByName.find(sampler->GetName()) == effect->samplersByName.end());
					effect->samplersByIndex[i] = sampler;
					effect->samplersByName[sampler->GetName()] = sampler;
				}				
			}
			else if (fourcc == 'VARB')
			{
				unsigned numBlocks = this->reader->ReadUInt();
				effect->varblocksByIndex = new EffectVarblock*[numBlocks];
				effect->numVarblocks = numBlocks;

				// load varblock, also 'extract' the variables defined within
				std::vector<EffectVariable*> vars;

				unsigned i, j;
				for (i = 0; i < numBlocks; i++)
				{
					EffectVarblock* varblock = this->varblockLoader.Load(reader, effect, vars);
					assert(effect->varblocksByName.find(varblock->GetName()) == effect->varblocksByName.end());
					effect->varblocksByIndex[i] = varblock;
					effect->varblocksByName[varblock->GetName()] = varblock;

					// copy old variables to newly resized array
					size_t curNumVars = effect->numVariables;
					effect->numVariables = curNumVars + vars.size();
					EffectVariable** newArray = new EffectVariable*[curNumVars + vars.size()];
					memcpy((void*)newArray, (void*)effect->variablesByIndex, sizeof(EffectVariable*) * curNumVars);

					// feed new array with new variables
					for (j = 0; j < vars.size(); j++)
					{
						assert(effect->variablesByName.find(vars[j]->GetName()) == effect->variablesByName.end());
						newArray[curNumVars + j] = vars[j];
						effect->variablesByName[vars[j]->GetName()] = vars[j];
					}

					// delete old array and set new array in effect
					delete [] effect->variablesByIndex;
					effect->variablesByIndex = newArray;

					vars.clear();
				}				
			}
            else if (fourcc == 'VRBF')
            {
                unsigned numBuffers = this->reader->ReadUInt();
                effect->varbuffersByIndex = new EffectVarbuffer*[numBuffers];
                effect->numVarbuffers = numBuffers;

                unsigned i;
                for (i = 0; i < numBuffers; i++)
                {
                    EffectVarbuffer* buffer = this->varbufferLoader.Load(reader, effect);
                    assert(effect->varbuffersByName.find(buffer->GetName()) == effect->varbuffersByName.end());
                    effect->varbuffersByIndex[i] = buffer;
                    effect->varbuffersByName[buffer->GetName()] = buffer;
                }
            }
			else if (this->reader->Eof())
			{
				// we're done, return
				break;
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
