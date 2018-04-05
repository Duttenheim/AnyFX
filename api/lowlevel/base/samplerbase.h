#pragma once
//------------------------------------------------------------------------------
/**
	Describes a basic sampler object.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
#include "variablebase.h"
#include "enums.h"
#include <vector>
namespace AnyFX
{
struct SamplerBase : public Annotable
{
public:
	/// constructor
	SamplerBase();
	/// destructor
	virtual ~SamplerBase();

	struct SamplerSettings
	{
		FilterMode filterMode;
		AddressMode addressU;
		AddressMode addressV;
		AddressMode addressW;
		ComparisonFunc comparisonFunc;

		bool isComparison;
		float minLod;
		float maxLod;
		float lodBias;
		float maxAnisotropic;
		float borderColor[4];

	} samplerSettings;

	std::string name;
	unsigned binding;
	unsigned set;
	std::vector<VariableBase*> textureVariables;

protected:
	friend class SamplerLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX