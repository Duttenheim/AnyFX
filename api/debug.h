#pragma once
#include <stdio.h>
#include <cstdarg>
#include <cstdlib>
//------------------------------------------------------------------------------
/**
	Debug macros.

	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
inline void
Error(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	vprintf(msg, argList);
	va_end(argList);
	std::abort();
}

} // namespace AnyFX
