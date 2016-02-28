//------------------------------------------------------------------------------
//  effectvariable.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "./effectvariable.h"
#include "internal/internaleffectvariable.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
EffectVariable::EffectVariable() :
	internalVariable(0),
	currentValue(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
EffectVariable::~EffectVariable()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectVariable::Discard()
{
    this->internalVariable->Release();
	this->internalVariable = 0;
}

//------------------------------------------------------------------------------
/**
*/
const eastl::string& 
EffectVariable::GetName() const
{
	return this->internalVariable->GetName();
}

//------------------------------------------------------------------------------
/**
*/
const VariableType& 
EffectVariable::GetType() const
{
	return this->internalVariable->GetType();
}

//------------------------------------------------------------------------------
/**
*/
const bool 
EffectVariable::IsInVarblock() const
{
	return this->internalVariable->IsInVarblock();
}

//------------------------------------------------------------------------------
/**
*/
const bool 
EffectVariable::IsSubroutine() const
{
    return this->internalVariable->IsSubroutine();
}

//------------------------------------------------------------------------------
/**
*/
void 
EffectVariable::Commit()
{
	this->internalVariable->Commit();
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetFloat(float f)
{
	this->internalVariable->SetFloat(f);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetFloat2(const float* vec)
{
	this->internalVariable->SetFloat2(vec);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetFloat4(const float* vec)
{
	this->internalVariable->SetFloat4(vec);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetFloatArray(const float* f, size_t count)
{
	this->internalVariable->SetFloatArray(f, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetFloat2Array(const float* f, size_t count)
{
	this->internalVariable->SetFloat2Array(f, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetFloat4Array(const float* f, size_t count)
{
	this->internalVariable->SetFloat4Array(f, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetInt(int i)
{
	this->internalVariable->SetInt(i);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetInt2(const int* vec)
{
	this->internalVariable->SetInt2(vec);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetInt4(const int* vec)
{
	this->internalVariable->SetInt4(vec);
}
//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetIntArray(const int* i, size_t count)
{
	this->internalVariable->SetIntArray(i, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetInt2Array(const int* i, size_t count)
{
	this->internalVariable->SetInt2Array(i, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetInt4Array(const int* i, size_t count)
{
	this->internalVariable->SetInt4Array(i, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetBool(bool b)
{
	this->internalVariable->SetBool(b);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetBool2(const bool* vec)
{
	this->internalVariable->SetBool2(vec);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetBool4(const bool* vec)
{
	this->internalVariable->SetBool4(vec);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetBoolArray(const bool* b, size_t count)
{
	this->internalVariable->SetBoolArray(b, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetBool2Array(const bool* b, size_t count)
{
	this->internalVariable->SetBool2Array(b, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetBool4Array(const bool* b, size_t count)
{
	this->internalVariable->SetBool4Array(b, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetMatrix(const float* mat)
{
	this->internalVariable->SetMatrix(mat);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetMatrixArray(const float* mat, size_t count)
{
	this->internalVariable->SetMatrixArray(mat, count);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetTexture(void* handle)
{
	this->internalVariable->SetTexture(handle);
}

//------------------------------------------------------------------------------
/**
*/
void
EffectVariable::SetTextureHandle(void* handle)
{
	// hmm, code should go here, but it hasn't
}

//------------------------------------------------------------------------------
/**
*/
const bool 
EffectVariable::IsActive() const
{
	return this->internalVariable->active;
}

} // namespace AnyFX