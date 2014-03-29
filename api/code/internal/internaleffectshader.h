#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::InternalEffectShader
    
    EffectShader backend, inherit this class to provide an implementation
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "effectshader.h"
#include <string>
namespace AnyFX
{
class Effect;
class InternalEffectShader
{
public:

	enum InternalShaderType
	{
		VertexShader,
		PixelShader,
		GeometryShader,
		HullShader,
		DomainShader,		
		ComputeShader,

		NumInternalShaderTypes
	};

	/// constructor
	InternalEffectShader();
	/// destructor
	virtual ~InternalEffectShader();

protected:
	friend class EffectShader;
	friend class EffectShaderStreamLoader;
	friend class EffectProgramStreamLoader;

	/// compiles shader with given code
	virtual bool Compile();

	/// returns name
	const std::string& GetName() const;
	/// returns error string
	const std::string& GetError() const;
	/// returns code
	const std::string& GetCode() const;

	Effect* effect;
	InternalShaderType type;
	std::string sourceCode;
	std::string name;
	std::string error;
}; 


//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
InternalEffectShader::GetName() const
{
	return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
InternalEffectShader::GetError() const
{
	return this->error;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
InternalEffectShader::GetCode() const
{
	return this->sourceCode;
}

} // namespace AnyFX
//------------------------------------------------------------------------------