#pragma once
//------------------------------------------------------------------------------
/**
    @file vartypes.h
    
    Holds shared enum for variable types
    
    (C) 2013 Gustav Sterbrant
*/

//------------------------------------------------------------------------------

#include <string>
// some thought it would be a good idea to define bool somewhere in some lib, so we undefine it here...
#undef Bool

namespace AnyFX
{

// make sure to keep this enum matched with the one in the compiler
enum VariableType
{
	Float,
	Float2,
	Float3,
	Float4,
	Double,
	Double2,
	Double3,
	Double4,
	Integer,
	Integer2,
	Integer3,
	Integer4,
	UInteger,
	UInteger2,
	UInteger3,
	UInteger4,
	Short,
	Short2,
	Short3,
	Short4,
	Bool,
	Bool2,
	Bool3,
	Bool4,
	Matrix2x2,
	Matrix2x3,
	Matrix2x4,
	Matrix3x2,
	Matrix3x3,
	Matrix3x4,
	Matrix4x2,
	Matrix4x3,
	Matrix4x4,
	AtomicCounter,
	TextureHandle,
	ImageHandle,
	SamplerHandle,
	Sampler1D,
	Sampler1DArray,
	Sampler2D,
	Sampler2DArray,
	Sampler2DMS,
	Sampler2DMSArray,
	Sampler3D,
	SamplerCube,
	SamplerCubeArray,
	Image1D,
	Image1DArray,
	Image2D,
	Image2DArray,
	Image2DMS,
	Image2DMSArray,
	Image3D,
	ImageCube,
	ImageCubeArray,
	Texture1D,
	Texture1DArray,
	Texture2D,
	Texture2DArray,
	Texture2DMS,
	Texture2DMSArray,
	Texture3D,
	TextureCube,
	TextureCubeArray,
	InputAttachment,
	InputAttachmentMS,
	InputAttachmentInteger,
	InputAttachmentIntegerMS,
	InputAttachmentUInteger,
	InputAttachmentUIntegerMS,
	Void,
	String,
	UserType,

	Undefined,

	NumTypes
};

//------------------------------------------------------------------------------
/**
*/
static std::string 
TypeToString(const VariableType& type)
{
	switch (type)
	{
	case Float:
		return "f";
	case Float2:
		return "f2";
	case Float3:
		return "f3";
	case Float4:
		return "f4";
	case Double:
		return "d";
	case Double2:
		return "d2";
	case Double3:
		return "d3";
	case Double4:
		return "d4";
	case Integer:
		return "i";
	case Integer2:
		return "i2";
	case Integer3:
		return "i3";
	case Integer4:
		return "i4";
	case UInteger:
		return "ui";
	case UInteger2:
		return "ui2";
	case UInteger3:
		return "ui3";
	case UInteger4:
		return "ui4";
	case Short:
		return "s";
	case Short2:
		return "s2";
	case Short3:
		return "s3";
	case Short4:
		return "s4";
	case Bool:
		return "b";
	case Bool2:
		return "b2";
	case Bool3:
		return "b3";
	case Bool4:
		return "b4";
	case Matrix2x2:
		return "m22";
	case Matrix2x3:
		return "m23";
	case Matrix2x4:
		return "m24";
	case Matrix3x2:
		return "m32";
	case Matrix3x3:
		return "m33";
	case Matrix3x4:
		return "m34";
	case Matrix4x2:
		return "m42";
	case Matrix4x3:
		return "m43";
	case Matrix4x4:
		return "m44";
	case AtomicCounter:
		return "ac";
	case TextureHandle:
		return "th";
	case ImageHandle:
		return "ih";
	case SamplerHandle:
		return "sh";
	case Sampler1D:
		return "s1d";
	case Sampler1DArray:
		return "s1da";
	case Sampler2D:
		return "s2d";
	case Sampler2DArray:
		return "s2da";
	case Sampler2DMS:
		return "s2dm";
	case Sampler2DMSArray:
		return "s2dma";
	case Sampler3D:
		return "s3d";
	case SamplerCube:
		return "sc";
	case SamplerCubeArray:
		return "sca";
	case Image1D:
		return "i1d";
	case Image1DArray:
		return "i1da";
	case Image2D:
		return "i2d";
	case Image2DArray:
		return "i2da";
	case Image2DMS:
		return "i2dm";
	case Image2DMSArray:
		return "i2dma";
	case Image3D:
		return "i3d";
	case ImageCube:
		return "ic";
	case ImageCubeArray:
		return "ica";
	case Void:
		return "v";
	case String:
		return "str";
	case UserType:
		return "user";
	default:
		return "undefined";
	}
}

//------------------------------------------------------------------------------
/**
*/
static unsigned
TypeToByteSize(const VariableType& type)
{
	switch (type)
	{
	case Float:
		return sizeof(float);
	case Float2:
		return 2 * sizeof(float);
	case Float3:
		return 3 * sizeof(float);
	case Float4:
		return 4 * sizeof(float);
	case Double:
		return sizeof(double);
	case Double2:
		return 2 * sizeof(double);
	case Double3:
		return 3 * sizeof(double);
	case Double4:
		return 4 * sizeof(double);
	case TextureHandle:
	case ImageHandle:
	case SamplerHandle:
	case UInteger:
	case Integer:
		return sizeof(int);
	case UInteger2:
	case Integer2:
		return 2 * sizeof(int);
	case UInteger3:
	case Integer3:
		return 3 * sizeof(int);
	case UInteger4:
	case Integer4:
		return 4 * sizeof(int);
	case Short:
		return sizeof(short);
	case Short2:
		return 2 * sizeof(short);
	case Short3:
		return 3 * sizeof(short);
	case Short4:
		return 4 * sizeof(short);

		// GPUs treat bools as integers, so just assume the size is 1 byte
	case Bool:
		return sizeof(bool);
	case Bool2:
		return 2 * sizeof(bool);
	case Bool3:
		return 3 * sizeof(bool);
	case Bool4:
		return 4 * sizeof(bool);
	case Matrix2x2:
		return 4 * sizeof(float);
	case Matrix2x3:
		return 6 * sizeof(float);
	case Matrix2x4:
		return 8 * sizeof(float);
	case Matrix3x2:
		return 6 * sizeof(float);
	case Matrix3x3:
		return 9 * sizeof(float);
	case Matrix3x4:
		return 12 * sizeof(float);
	case Matrix4x2:
		return 8 * sizeof(float);
	case Matrix4x3:
		return 12 * sizeof(float);
	case Matrix4x4:
		return 16 * sizeof(float);
	default:		// this is for all special types, such as texture handles etc.
		return sizeof(int);
	}
}

} // namespace AnyFX
//------------------------------------------------------------------------------
