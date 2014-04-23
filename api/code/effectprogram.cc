//------------------------------------------------------------------------------
//  effectprogram.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "./effectprogram.h"
#include "./effectrenderstate.h"
#include "./effectshader.h"
#include "internal/internaleffectprogram.h"
#include <assert.h>


namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectProgram::EffectProgram() :
	internalProgram(NULL)
{
	this->shaderBlock.vs = NULL;
	this->shaderBlock.ps = NULL;
	this->shaderBlock.hs = NULL;
	this->shaderBlock.ds = NULL;
	this->shaderBlock.gs = NULL;
	this->shaderBlock.cs = NULL;
}

//------------------------------------------------------------------------------
/**
*/
EffectProgram::~EffectProgram()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectProgram::Discard()
{
	delete this->internalProgram;
	this->internalProgram = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectProgram::Apply()
{
	assert(0 != this->renderState);
	assert(0 != this->internalProgram);
	this->internalProgram->Apply();	
	this->renderState->Apply();	
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectProgram::Commit()
{
	assert(0 != this->internalProgram);
	this->internalProgram->Commit();
}

//------------------------------------------------------------------------------
/**
*/
const std::string& 
EffectProgram::GetName() const
{
	assert(0 != this->internalProgram);
	return this->internalProgram->GetName();
}

//------------------------------------------------------------------------------
/**
*/
bool 
EffectProgram::IsValid()
{
	return this->internalProgram->linkState == InternalEffectProgram::LinkedOk;
}

//------------------------------------------------------------------------------
/**
*/
const std::string& 
EffectProgram::GetError() const
{
	return this->internalProgram->error;
}

//------------------------------------------------------------------------------
/**
*/
const bool 
EffectProgram::SupportsTessellation() const
{
	return this->internalProgram->SupportsTessellation();
}

//------------------------------------------------------------------------------
/**
*/
const unsigned* 
EffectProgram::GetLocalSizes() const
{
    assert(0 != this->shaderBlock.cs);
    return this->shaderBlock.cs->GetLocalSizes();
}
} // namespace AnyFX
