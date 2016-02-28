#pragma once
//------------------------------------------------------------------------------
/**
	Lists enums used by the compiler do describe flags
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------

namespace AnyFX
{

enum Implementation
{
	HLSL,
	GLSL,
	SPIRV,
	Metal,
	PS,
	Wii,

	InvalidType,

	NumImplementations
};

enum ImageFormat
{
	RGBA32F,
	RGBA16F,
	RG32F,
	RG16F,
	R11G11B10F,
	R32F,
	R16F,
	RGBA16,
	RGB10A2,
	RGBA8,
	RG16,
	RG8,
	R16,
	R8,
	RGBA16SNORM,
	RGBA8SNORM,
	RG16SNORM,
	RG8SNORM,
	R16SNORM,
	R8SNORM,
	RGBA32I,
	RGBA16I,
	RGBA8I,
	RG32I,
	RG16I,
	RG8I,
	R32I,
	R16I,
	R8I,
	RGBA32UI,
	RGBA16UI,
	RGB10A2UI,
	RGBA8UI,
	RG32UI,
	RG16UI,
	RG8UI,
	R32UI,
	R16UI,
	R8UI,
	NoFormat,

	NumImageFormats
};

enum AccessMode
{
	Read,
	Write,
	ReadWrite,
	NoAccess,

	NumImageAccessModes
};

enum AddressMode
{
	Wrap = 0,
	Mirror,
	Clamp,
	Border,
	MirrorOnce,

	NumAddressModes
};

enum FilterMode
{
	MinMagMipPoint = 0,
	MinMagMipLinear,
	MinMagPointMipLinear,
	MinMipPointMagLinear,
	MinPointMipMagLinear,
	MinLinearMipMagPoint,
	MinMipLinearMagPoint,
	MinMagLinearMipPoint,
	Anisotropic,
	PointAll,
	LinearAll,

	NumFilterModes
};

enum ComparisonFunc
{
	Never = 0,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	Equal,
	NotEqual,
	Always,

	NumDrawDepthFuncs
};

enum BlendMode
{
	ZeroBlend,
	OneBlend,
	SourceColor,
	OneMinusSourceColor,
	DestinationColor,
	OneMinusDestinationColor,
	SourceAlpha,
	OneMinusSourceAlpha,
	DestinationAlpha,
	OneMinusDestinationAlpha,
	SourceAlphaSaturate,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha,

	NumBlendModes
};

enum BlendOperation
{
	Add,
	Sub,
	InvSub,
	Min,
	Max,

	NumBlendOperations
};

enum BlendBoolFlag
{
	BlendEnabled,

	NumBlendBoolFlags
};

enum BlendModeFlag
{
	SrcBlend,
	DstBlend,
	SrcBlendAlpha,
	DstBlendAlpha,

	NumBlendModeFlags
};

enum BlendFuncFlag
{
	BlendOp,
	BlendOpAlpha,

	NumBlendFlags
};

enum StencilOperation
{
	Keep,
	Zero,
	Replace,
	Increase,
	IncreaseWrap,
	Decrease,
	DecreaseWrap,
	Invert,

	NumStencilOperations
};

enum StencilOpFlag
{
	StencilFrontFailOp,
	StencilBackFailOp,
	StencilFrontPassOp,
	StencilBackPassOp,
	StencilFrontDepthFailOp,
	StencilBackDepthFailOp,

	NumStencilOpFlags
};

enum StencilFuncFlag
{
	StencilFrontFunc,
	StencilBackFunc,

	NumStencilFuncFlags
};

enum DrawBoolFlag
{
	DepthEnabled,
	DepthWrite,
	DepthRead,
	SeparateBlend,
	StencilEnabled,
	AlphaToCoverageEnabled,
	MultisampleEnabled,
	RasterizerDiscardEnabled,
	LogicOpEnabled,
	PolygonOffsetEnabled,

	NumDrawBoolFlags
};

enum DrawIntFlag
{
	StencilReadMask,
	StencilWriteMask,
	DepthFunc,
	CullMode,
	FillMode,

	NumDrawIntFlags
};

enum DrawCullMode
{
	Back,
	Front,
	None,

	NumDrawCullModes
};

enum DrawFillMode
{
	Solid,
	Line,
	Point,

	NumDrawFillModes
};

enum DrawWindingMode
{
	CW,
	CCW,

	NumWindingModes
};

enum DrawSampleCount
{
	Bits1,
	Bits2,
	Bits4,
	Bits8,
	Bits16,
	Bits32,
	Bits64,

	NumSampleCounts
};

enum DrawLogicalOperation
{
	Clear,
	And,
	AndReverse,
	Copy,
	AndInverted,
	NoOp,
	Xor,
	Or,
	Nor,
	Equivalent,
	Inverted,
	OrReverse,
	CopyInverted,
	OrInverted,
	Nand,
	Set
};

enum ShaderType
{
	VertexShader,
	PixelShader,
	GeometryShader,
	HullShader,
	DomainShader,
	ComputeShader,

	NumInternalShaderTypes
};

} // namespace AnyFX