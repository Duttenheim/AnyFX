#pragma once
//------------------------------------------------------------------------------
/**
	Describes a basic shader object.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "annotable.h"
#include <string>
namespace AnyFX
{
struct ShaderBase : public Annotable
{
public:
	/// constructor
	ShaderBase();
	/// destructor
	virtual ~ShaderBase();

	int type;
	std::string sourceCode;
	std::string name;
	std::string error;
	std::string warning;
	unsigned localSizes[3];

protected:
	friend class ShaderLoader;

	/// callback for when program is done loading
	virtual void OnLoaded();
};
} // namespace AnyFX