#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectSampler
    
    An effect sampler provides an interface to which one can alter samplers.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------

#ifdef AFX_API
#error "afxapi.h included before effectsampler.h"
#endif

#include <string>
#include "annotable.h"
namespace AnyFX
{
class InternalEffectSampler;
class EffectSampler : public Annotable
{
public:

	enum AddressMode
	{
		Wrap,
		Mirror,
		Clamp,
		Border,
		MirrorOnce,

		NumAddressModes
	};

	enum FilterMode
	{
		MinMagMipPoint,
		MinMagMipLinear,
		MinMagPointMipLinear,
		MinMipPointMagLinear,
		MinPointMipMagLinear,
		MinLinearMipMagPoint,
		MinMipLinearMagPoint,
		MinMagLinearMipPoint,
		Anisotropic,
		Point,
		Linear,

		NumFilterModes
	};

	enum ComparisonFunc
	{
		Never,
		Less,
		LessEqual,
		Greater,
		GreaterEqual,
		Equal,
		NotEqual,		
		Always,

		NumDrawDepthFuncs
	};

	/// constructor
	EffectSampler();
	/// destructor
	virtual ~EffectSampler();

	/// discard program
	void Discard();

	/// get name of variable
	const std::string& GetName() const;

private:
	friend class EffectSamplerStreamLoader;
	friend class InternalEffectProgram;

	InternalEffectSampler* internalSampler;
}; 




} // namespace AnyFX
//------------------------------------------------------------------------------