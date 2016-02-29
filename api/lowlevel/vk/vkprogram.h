#pragma once
//------------------------------------------------------------------------------
/**
	Describes a base program object as written in the AnyFX shader file.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "base/programbase.h"
namespace AnyFX
{
struct VkProgram : public ProgramBase
{
public:
	/// constructor
	VkProgram();
	/// destructor
	virtual ~VkProgram();
	
private:
	/// handle loading
	void OnLoadingDone();
};
} // namespace AnyFX