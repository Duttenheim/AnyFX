//------------------------------------------------------------------------------
//  effectprogramstreamloader.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "effectprogramstreamloader.h"
#include "internal/internaleffectprogram.h"
#include "effectrenderstate.h"
#include "effectprogram.h"
#include "effectshader.h"
#include "effectsubroutine.h"
#include "effectvariable.h"
#include "effect.h"
#include <assert.h>
#include "internal/glsl4/glsl4effectprogram.h"
#include "effectannotationstreamloader.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectProgramStreamLoader::EffectProgramStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectProgramStreamLoader::~EffectProgramStreamLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectProgram* 
EffectProgramStreamLoader::Load( BinReader* reader, Effect* effect )
{
	InternalEffectProgram* internalProgram = 0;
	
	// we should create our implementation back-end first
	if (effect->GetType() == Effect::GLSL)
	{
		if (effect->GetMajor() == 4) internalProgram = new GLSL4EffectProgram;
	}
	internalProgram->effect = effect;

	EffectProgram* program = new EffectProgram;
	std::string name = reader->ReadString();
	internalProgram->name = name;

	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		EffectAnnotationStreamLoader loader;
		loader.Load(reader, program);
	}

	// read and set tessellation data
	bool supportsTessellation = reader->ReadBool();
	int patchSize = reader->ReadInt();
	internalProgram->supportsTessellation = supportsTessellation;
	internalProgram->patchSize = patchSize;

    // read and set transform feedback support
    bool supportsTransformFeedback = reader->ReadBool();
    internalProgram->supportsTransformFeedback = supportsTransformFeedback;

	int magic;
    unsigned numSubroutineMappings, i;
	magic = reader->ReadInt();
	assert('VERT' == magic);
	std::string vs = reader->ReadString();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString();
        std::string imp = reader->ReadString();

        // get subroutines
        EffectSubroutine* implementationVar = effect->GetSubroutineByName(imp);
        assert(implementationVar->GetType() == EffectSubroutine::Implementation);

        // assign in program
        internalProgram->shaderBlock.vsSubroutines[var] = implementationVar->internalSubroutine;
    }

	magic = reader->ReadInt();
	assert('PIXL' == magic);
	std::string ps = reader->ReadString();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString();
        std::string imp = reader->ReadString();

        // get subroutines
        EffectSubroutine* implementationVar = effect->GetSubroutineByName(imp);
        assert(implementationVar->GetType() == EffectSubroutine::Implementation);

        // assign in program
        internalProgram->shaderBlock.psSubroutines[var] = implementationVar->internalSubroutine;
    }

	magic = reader->ReadInt();
	assert('HULL' == magic);
	std::string hs = reader->ReadString();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString();
        std::string imp = reader->ReadString();

        // get subroutines
        EffectSubroutine* implementationVar = effect->GetSubroutineByName(imp);
        assert(implementationVar->GetType() == EffectSubroutine::Implementation);

        // assign in program
        internalProgram->shaderBlock.hsSubroutines[var] = implementationVar->internalSubroutine;
    }

	magic = reader->ReadInt();
	assert('DOMA' == magic);
	std::string ds = reader->ReadString();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString();
        std::string imp = reader->ReadString();

        // get subroutines
        EffectSubroutine* implementationVar = effect->GetSubroutineByName(imp);
        assert(implementationVar->GetType() == EffectSubroutine::Implementation);

        // assign in program
        internalProgram->shaderBlock.dsSubroutines[var] = implementationVar->internalSubroutine;
    }

	magic = reader->ReadInt();
	assert('GEOM' == magic);
	std::string gs = reader->ReadString();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString();
        std::string imp = reader->ReadString();

        // get subroutines
        EffectSubroutine* implementationVar = effect->GetSubroutineByName(imp);
        assert(implementationVar->GetType() == EffectSubroutine::Implementation);

        // assign in program
        internalProgram->shaderBlock.gsSubroutines[var] = implementationVar->internalSubroutine;
    }

	magic = reader->ReadInt();
	assert('COMP' == magic);
	std::string cs = reader->ReadString();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString();
        std::string imp = reader->ReadString();

        // get subroutines
        EffectSubroutine* implementationVar = effect->GetSubroutineByName(imp);
        assert(implementationVar->GetType() == EffectSubroutine::Implementation);

        // assign in program
        internalProgram->shaderBlock.csSubroutines[var] = implementationVar->internalSubroutine;
    }

	magic = reader->ReadInt();
	assert('RSTA' == magic);
	std::string rs = reader->ReadString();

	// find shaders previously loaded in the effect and attach them to this program
	if (!vs.empty())
	{
		EffectShader* vertexShader = effect->GetShaderByName(vs);
		program->shaderBlock.vs = vertexShader;
		internalProgram->shaderBlock.vs = vertexShader->internalShader;
	}
	
	if (!ps.empty())
	{
		EffectShader* pixelShader = effect->GetShaderByName(ps);	
		program->shaderBlock.ps = pixelShader;
		internalProgram->shaderBlock.ps = pixelShader->internalShader;
	}
	
	if (!hs.empty())
	{
		EffectShader* hullShader = effect->GetShaderByName(hs);
		program->shaderBlock.hs = hullShader;
		internalProgram->shaderBlock.hs = hullShader->internalShader;
	}

	if (!ds.empty())
	{
		EffectShader* domainShader = effect->GetShaderByName(ds);
		program->shaderBlock.ds = domainShader;
		internalProgram->shaderBlock.ds = domainShader->internalShader;
	}

	if (!gs.empty())
	{
		EffectShader* geometryShader = effect->GetShaderByName(gs);
		program->shaderBlock.gs = geometryShader;
		internalProgram->shaderBlock.gs = geometryShader->internalShader;
	}

	if (!cs.empty())
	{
		EffectShader* computeShader = effect->GetShaderByName(cs);
		program->shaderBlock.cs = computeShader;
		internalProgram->shaderBlock.cs = computeShader->internalShader;
	}

	// assert there is a render state
	assert(!rs.empty());
	EffectRenderState* renderState = effect->GetRenderStateByName(rs);
	program->renderState = renderState;
	internalProgram->renderState = renderState->internalRenderState;

	// bind internal representation to program
	program->internalProgram = internalProgram;

	// link program and load into effect if successful
	bool success = internalProgram->Link();
	if (success)
	{
		internalProgram->linkState = InternalEffectProgram::LinkedOk;
        internalProgram->SetupSubroutines();
	}
	else
	{
		internalProgram->linkState = InternalEffectProgram::LinkerError;
	}
	return program;
}

} // namespace AnyFX