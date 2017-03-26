#pragma once
//------------------------------------------------------------------------------
/**
    types.h
    
    Contains data types used to maintain initializers and data types
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "util.h"
namespace AnyFX
{

template<typename T, int S>
struct vector
{
	T v[S];
};

template<typename T, int R, int C>
struct matrix
{
	T m[R][C];
};

enum class Qualifiers : unsigned int
{
	None = 0,
	Shared = 1,			// resource should have the same layout despite the shader (useful for include headers)
	Push = 2,			// resource is a push-constant block
	RangeBind = 4,		// resource can be bound as a range of a buffer	
};
ENUM_OPERATORS(Qualifiers)

}
