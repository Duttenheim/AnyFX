//------------------------------------------------------------------------------
//  programloader.cc
//  (C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "programloader.h"
#include "annotationloader.h"
#include "base/programbase.h"
#include "base/renderstatebase.h"
#include "base/programbase.h"
#include "base/shaderbase.h"
#include "base/subroutinebase.h"
#include "base/variablebase.h"
#include "shadereffect.h"
#include "gl4/gl4program.h"
#include "vk/vkprogram.h"
#include "shadertypes.h"
#include <assert.h>

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
ProgramLoader::ProgramLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ProgramLoader::~ProgramLoader()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ProgramBase*
ProgramLoader::Load(BinReader* reader, ShaderEffect* effect)
{
	ProgramBase* program = 0;
	
	// we should create our implementation back-end first
	if (effect->header == Implementation::GLSL)
	{
		if (effect->major == 4) program = new GL4Program;
	}
	else if (effect->header == Implementation::SPIRV)
	{
		program = new VkProgram;
	}
	else
	{
		program = new ProgramBase;
	}
	
    std::string name = reader->ReadString().c_str();
	program->name = name;

	bool hasAnnotation = reader->ReadBool();
	if (hasAnnotation)
	{
		AnnotationLoader loader;
		loader.Load(reader, program);
	}

	// read and set tessellation data
	bool supportsTessellation = reader->ReadBool();
	unsigned patchSize = reader->ReadUInt();
	unsigned numVsInputs = reader->ReadUInt();
	unsigned numPsOutputs = reader->ReadUInt();
	program->supportsTessellation = supportsTessellation;
	program->patchSize = patchSize;
	program->numVsInputs = numVsInputs;
	program->numPsOutputs = numPsOutputs;

	program->vsInputSlots.resize(program->numVsInputs);
	unsigned i;
	for (i = 0; i < program->numVsInputs; i++)
	{
		program->vsInputSlots[i] = reader->ReadUInt();
	}

	program->psOutputSlots.resize(program->numPsOutputs);
	for (i = 0; i < program->numPsOutputs; i++)
	{
		program->psOutputSlots[i] = reader->ReadUInt();
	}

    // read and set transform feedback support
    bool supportsTransformFeedback = reader->ReadBool();
	program->supportsTransformFeedback = supportsTransformFeedback;

	int magic;
    unsigned numSubroutineMappings;
	magic = reader->ReadInt();
	assert('VERT' == magic);
    std::string vs = reader->ReadString().c_str();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString().c_str();
        std::string imp = reader->ReadString().c_str();
    }
	program->shaderBlock.vsBinarySize = reader->ReadUInt();
	program->shaderBlock.vsBinary = reader->ReadBytes(program->shaderBlock.vsBinarySize);

	magic = reader->ReadInt();
	assert('HULL' == magic);
    std::string hs = reader->ReadString().c_str();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString().c_str();
        std::string imp = reader->ReadString().c_str();
    }
	program->shaderBlock.hsBinarySize = reader->ReadUInt();
	program->shaderBlock.hsBinary = reader->ReadBytes(program->shaderBlock.hsBinarySize);

	magic = reader->ReadInt();
	assert('DOMA' == magic);
    std::string ds = reader->ReadString().c_str();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString().c_str();
        std::string imp = reader->ReadString().c_str();
    }
	program->shaderBlock.dsBinarySize = reader->ReadUInt();
	program->shaderBlock.dsBinary = reader->ReadBytes(program->shaderBlock.dsBinarySize);

	magic = reader->ReadInt();
	assert('GEOM' == magic);
    std::string gs = reader->ReadString().c_str();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString().c_str();
        std::string imp = reader->ReadString().c_str();
    }
	program->shaderBlock.gsBinarySize = reader->ReadUInt();
	program->shaderBlock.gsBinary = reader->ReadBytes(program->shaderBlock.gsBinarySize);

	magic = reader->ReadInt();
	assert('PIXL' == magic);
	std::string ps = reader->ReadString().c_str();
	numSubroutineMappings = reader->ReadUInt();
	for (i = 0; i < numSubroutineMappings; i++)
	{
		std::string var = reader->ReadString().c_str();
		std::string imp = reader->ReadString().c_str();
	}
	program->shaderBlock.psBinarySize = reader->ReadUInt();
	program->shaderBlock.psBinary = reader->ReadBytes(program->shaderBlock.psBinarySize);

	magic = reader->ReadInt();
	assert('COMP' == magic);
    std::string cs = reader->ReadString().c_str();
    numSubroutineMappings = reader->ReadUInt();
    for (i = 0; i < numSubroutineMappings; i++)
    {
        std::string var = reader->ReadString().c_str();
        std::string imp = reader->ReadString().c_str();
    }
	program->shaderBlock.csBinarySize = reader->ReadUInt();
	program->shaderBlock.csBinary = reader->ReadBytes(program->shaderBlock.csBinarySize);

	// read names of active blocks
	unsigned numActiveBlocks = reader->ReadUInt();
	for (i = 0; i < numActiveBlocks; i++)
	{
		program->activeVarblockNames.insert(reader->ReadString().c_str());
	}

	// read names of active variables
	unsigned numActiveVars = reader->ReadUInt();
	for (i = 0; i < numActiveVars; i++)
	{
		program->activeVariableNames.insert(reader->ReadString().c_str());
	}

	// read variable buffer offsets
	unsigned numOffsets = reader->ReadUInt();
	for (i = 0; i < numOffsets; i++)
	{
		std::string var = reader->ReadString().c_str();
		unsigned offset = reader->ReadUInt();
		program->variableBlockOffsets[var] = offset;
	}

	magic = reader->ReadInt();
	assert('RSTA' == magic);
    std::string rs = reader->ReadString().c_str();

	// find shaders previously loaded in the effect and attach them to this program
	if (!vs.empty())
	{
		ShaderBase* vertexShader = effect->GetShader(vs);
		program->shaderBlock.vs = vertexShader;
		program->valid = true;
	}
	
	if (!ps.empty())
	{
		ShaderBase* pixelShader = effect->GetShader(ps);
		program->shaderBlock.ps = pixelShader;
		if (!vs.empty()) program->valid = true;
	}
	
	if (!hs.empty())
	{
		ShaderBase* hullShader = effect->GetShader(hs);
		program->shaderBlock.hs = hullShader;
		if (!vs.empty()) program->valid = true;
	}

	if (!ds.empty())
	{
		ShaderBase* domainShader = effect->GetShader(ds);
		program->shaderBlock.ds = domainShader;
		if (!vs.empty() && !hs.empty()) program->valid = true;
	}

	if (!gs.empty())
	{
		ShaderBase* geometryShader = effect->GetShader(gs);
		program->shaderBlock.gs = geometryShader;
		if (!vs.empty()) program->valid = true;
	}

	if (!cs.empty())
	{
		ShaderBase* computeShader = effect->GetShader(cs);
		program->shaderBlock.cs = computeShader;
		program->valid = true;
	}

	// assert there is a render state
	assert(!rs.empty());
	RenderStateBase* renderState = effect->GetRenderState(rs);
	program->renderState = renderState;
	program->OnLoaded();
	return program;
}

} // namespace AnyFX