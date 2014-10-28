//------------------------------------------------------------------------------
//  glsl4effectrenderstate.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "glsl4effectrenderstate.h"

namespace AnyFX
{

/// define a conversion table to map blend modes from AnyFX to OpenGL
static const GLenum opengl4BlendTable[] = 
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC_ALPHA_SATURATE,
	GL_SRC1_COLOR,
	GL_ONE_MINUS_SRC1_COLOR,
	GL_SRC1_ALPHA,
	GL_ONE_MINUS_SRC1_ALPHA
};

static const GLenum opengl4EquationTable[] = 
{
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX
};

static const GLenum opengl4ComparisonTable[] = 
{
	GL_NEVER,
	GL_LESS,
	GL_LEQUAL,
	GL_GREATER,
	GL_GEQUAL,
	GL_EQUAL,
	GL_NOTEQUAL,	
	GL_ALWAYS
};

static const GLenum opengl4StencilOpTable[] = 
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_INCR_WRAP,
	GL_DECR,
	GL_DECR_WRAP,
	GL_INVERT
};

static const GLenum opengl4FaceTable[] = 
{
	GL_BACK,
	GL_FRONT,
	GL_FRONT_AND_BACK
};

static const GLenum opengl4PolygonModeTable[] =
{
	GL_FILL,
	GL_LINE,
	GL_POINT
};



// set global state to a sane default
unsigned GLSL4GlobalRenderState::polygonMode = GL_INVALID_ENUM;
unsigned GLSL4GlobalRenderState::cullMode = GL_INVALID_ENUM;
unsigned GLSL4GlobalRenderState::cullFace = GL_INVALID_ENUM;
bool GLSL4GlobalRenderState::depthEnabled = true;
bool GLSL4GlobalRenderState::depthWriteEnabled = true;
bool GLSL4GlobalRenderState::depthClampEnabled = true;
unsigned GLSL4GlobalRenderState::depthFunc = GL_INVALID_ENUM;
bool GLSL4GlobalRenderState::scissorEnabled = false;
bool GLSL4GlobalRenderState::multisampleEnabled = false;
bool GLSL4GlobalRenderState::alphaToCoverageEnabled = false;

bool GLSL4GlobalRenderState::stencilEnabled = false;
unsigned GLSL4GlobalRenderState::stencilBackOp[3] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };
unsigned GLSL4GlobalRenderState::stencilFrontOp[3] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };
unsigned GLSL4GlobalRenderState::stencilReadMask = -1;
unsigned GLSL4GlobalRenderState::stencilWriteMask = -1;
unsigned GLSL4GlobalRenderState::stencilFunc[2] = {GL_ALWAYS, GL_ALWAYS};
unsigned GLSL4GlobalRenderState::stencilRef[2] = {0, 0};

bool GLSL4GlobalRenderState::blendEnabled[InternalEffectRenderState::MaxNumRenderTargets] = { GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE };
unsigned GLSL4GlobalRenderState::srcBlends[InternalEffectRenderState::MaxNumRenderTargets] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };
unsigned GLSL4GlobalRenderState::dstBlends[InternalEffectRenderState::MaxNumRenderTargets] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };
unsigned GLSL4GlobalRenderState::alphaSrcBlends[InternalEffectRenderState::MaxNumRenderTargets] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };
unsigned GLSL4GlobalRenderState::alphaDstBlends[InternalEffectRenderState::MaxNumRenderTargets] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };
unsigned GLSL4GlobalRenderState::blendOps[InternalEffectRenderState::MaxNumRenderTargets] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };
unsigned GLSL4GlobalRenderState::alphaBlendOps[InternalEffectRenderState::MaxNumRenderTargets] = { GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM, GL_INVALID_ENUM };


#define APPLY_POLYGON_MODE(mode) \
    if (GLSL4GlobalRenderState::polygonMode != mode) \
    { \
        GLSL4GlobalRenderState::polygonMode = mode; \
        glPolygonMode(GL_FRONT_AND_BACK, opengl4PolygonModeTable[mode]); \
    }

#define APPLY_CULL_MODE(mode) \
    if (GLSL4GlobalRenderState::cullMode != mode) \
    { \
        GLSL4GlobalRenderState::cullMode = mode; \
        if (mode != EffectRenderState::None) \
        { \
            glEnable(GL_CULL_FACE); \
            glCullFace(opengl4FaceTable[mode]); \
        } \
        else \
        { \
            glDisable(GL_CULL_FACE); \
        } \
    }

#define APPLY_STENCIL_MODE(enabled, backFailOp, backDepthFailOp, backPassOp, frontFailOp, frontDepthFailOp, frontPassOp, backFaceFunc, frontFaceFunc, frontRef, backRef, writeMask, readMask) \
    if (enabled) \
    { \
        if (enabled != GLSL4GlobalRenderState::stencilEnabled) \
        { \
            GLSL4GlobalRenderState::stencilEnabled = enabled; \
            glEnable(GL_STENCIL_TEST); \
        } \
        if (backFailOp != GLSL4GlobalRenderState::stencilBackOp[0] || backDepthFailOp != GLSL4GlobalRenderState::stencilBackOp[1] || backPassOp != GLSL4GlobalRenderState::stencilBackOp[2]) \
        { \
            GLSL4GlobalRenderState::stencilBackOp[0] = backFailOp; \
            GLSL4GlobalRenderState::stencilBackOp[1] = backDepthFailOp; \
            GLSL4GlobalRenderState::stencilBackOp[2] = backPassOp; \
            glStencilOpSeparate(	                               \
                GL_BACK,                                           \
                opengl4StencilOpTable[backFailOp],                 \
                opengl4StencilOpTable[backDepthFailOp],            \
                opengl4StencilOpTable[backPassOp]);                \
        } \
        if (frontFailOp != GLSL4GlobalRenderState::stencilFrontOp[0] || frontDepthFailOp != GLSL4GlobalRenderState::stencilFrontOp[1] || frontPassOp != GLSL4GlobalRenderState::stencilFrontOp[2]) \
        { \
            GLSL4GlobalRenderState::stencilFrontOp[0] = frontFailOp; \
            GLSL4GlobalRenderState::stencilFrontOp[1] = frontDepthFailOp; \
            GLSL4GlobalRenderState::stencilFrontOp[2] = frontPassOp; \
            glStencilOpSeparate(	                               \
                GL_FRONT,                                          \
                opengl4StencilOpTable[frontFailOp],                \
                opengl4StencilOpTable[frontDepthFailOp],           \
                opengl4StencilOpTable[frontPassOp]);               \
        } \
        if (backFaceFunc != GLSL4GlobalRenderState::stencilFunc[0] || backRef != GLSL4GlobalRenderState::stencilRef[0]) \
        { \
            GLSL4GlobalRenderState::stencilFunc[0] = backFaceFunc; \
            GLSL4GlobalRenderState::stencilRef[0] = backRef; \
            glStencilFuncSeparate(                                 \
                GL_BACK,                                           \
                opengl4ComparisonTable[backFaceFunc],              \
                backRef,                                           \
                readMask);                                         \
        } \
        if (frontFaceFunc != GLSL4GlobalRenderState::stencilFunc[1] || frontRef != GLSL4GlobalRenderState::stencilRef[1]) \
        { \
            GLSL4GlobalRenderState::stencilFunc[0] = frontFaceFunc; \
            GLSL4GlobalRenderState::stencilRef[0] = frontRef; \
            glStencilFuncSeparate(                                 \
                GL_FRONT,                                          \
                opengl4ComparisonTable[frontFaceFunc],             \
                frontRef,                                          \
                readMask);                                         \
        } \
        if (writeMask != GLSL4GlobalRenderState::stencilWriteMask) \
        { \
            GLSL4GlobalRenderState::stencilWriteMask = writeMask;        \
            glStencilMask(writeMask); \
        } \
    } \
    else \
    { \
        if (enabled != GLSL4GlobalRenderState::stencilEnabled) \
        { \
            GLSL4GlobalRenderState::stencilEnabled = enabled; \
            glDisable(GL_STENCIL_TEST); \
        } \
    }

#define APPLY_DEPTH_MODE(enabled, write, clamp, func) \
    if (enabled) \
    { \
        if (enabled != GLSL4GlobalRenderState::depthEnabled) \
        { \
            GLSL4GlobalRenderState::depthEnabled = enabled; \
            glEnable(GL_DEPTH_TEST); \
        } \
        if (write != GLSL4GlobalRenderState::depthWriteEnabled) \
        { \
            GLSL4GlobalRenderState::depthWriteEnabled = write; \
            glDepthMask(write ? GL_TRUE : GL_FALSE); \
        } \
        if (func != GLSL4GlobalRenderState::depthFunc) \
        { \
            GLSL4GlobalRenderState::depthFunc = func; \
            glDepthFunc(opengl4ComparisonTable[func]); \
        } \
    } \
    else \
    { \
        if (enabled != GLSL4GlobalRenderState::depthEnabled) \
        { \
            GLSL4GlobalRenderState::depthEnabled = enabled; \
            glDisable(GL_DEPTH_TEST); \
        } \
    } \
	if (clamp) \
	{ \
		if (clamp != GLSL4GlobalRenderState::depthClampEnabled) \
		{ \
			GLSL4GlobalRenderState::depthClampEnabled = clamp; \
			glEnable(GL_DEPTH_CLAMP); \
		} \
	} \
	else \
	{ \
		if (clamp != GLSL4GlobalRenderState::depthClampEnabled) \
		{ \
			GLSL4GlobalRenderState::depthClampEnabled = clamp; \
			glDisable(GL_DEPTH_CLAMP); \
		} \
	}

#define APPLY_BLEND_MODE(enabled, separate, srcBlend, dstBlend, srcAlphaBlend, dstAlphaBlend, blendOp, alphaBlendOp, index) \
    if (enabled != GLSL4GlobalRenderState::blendEnabled[index]) \
    { \
        GLSL4GlobalRenderState::blendEnabled[index] = enabled; \
        if (enabled) \
        { \
            glEnablei(GL_BLEND, index); \
            if (srcBlend != GLSL4GlobalRenderState::srcBlends[index] || dstBlend != GLSL4GlobalRenderState::dstBlends[index] || srcAlphaBlend != GLSL4GlobalRenderState::alphaSrcBlends[index] || dstAlphaBlend != GLSL4GlobalRenderState::alphaDstBlends[index]) \
            { \
                GLSL4GlobalRenderState::srcBlends[index] = srcBlend;               \
                GLSL4GlobalRenderState::dstBlends[index] = dstBlend;               \
                GLSL4GlobalRenderState::alphaSrcBlends[index] = srcAlphaBlend;     \
                GLSL4GlobalRenderState::alphaDstBlends[index] = dstAlphaBlend;     \
                if (separate) \
                { \
                    glBlendFuncSeparatei(                                       \
                        index,                                                  \
                        opengl4BlendTable[srcBlend],                            \
                        opengl4BlendTable[dstBlend],                            \
                        opengl4BlendTable[srcAlphaBlend],                       \
                        opengl4BlendTable[dstAlphaBlend]);                      \
                } \
                else \
                { \
                    glBlendFunci(                                               \
                        index,                                                  \
                        opengl4BlendTable[srcBlend],                            \
                        opengl4BlendTable[dstBlend]);                           \
                } \
            } \
            if (blendOp != GLSL4GlobalRenderState::blendOps[index] || alphaBlendOp != GLSL4GlobalRenderState::alphaBlendOps[index]) \
            { \
                GLSL4GlobalRenderState::blendOps[index] = blendOp;             \
                GLSL4GlobalRenderState::alphaBlendOps[index] = alphaBlendOp;   \
                if (separate) \
                { \
                    glBlendEquationSeparatei(                       \
                        index,                                      \
                        opengl4EquationTable[blendOp],              \
                        opengl4EquationTable[alphaBlendOp]);        \
                } \
                else \
                { \
                    glBlendEquationi(                               \
                        index,                                      \
                        opengl4EquationTable[blendOp]);             \
                } \
            } \
        } \
        else \
        { \
            glDisablei(GL_BLEND, index); \
        } \
    }

#define APPLY_SCISSOR_MODE(enabled) \
    if (enabled != GLSL4GlobalRenderState::scissorEnabled) \
    { \
        GLSL4GlobalRenderState::scissorEnabled = enabled; \
        if (enabled) \
        { \
            glEnable(GL_SCISSOR_TEST); \
        } \
        else \
        { \
            glDisable(GL_SCISSOR_TEST); \
        } \
    }

#define APPLY_ALPHA_TO_COVERAGE(enabled) \
    if (enabled != GLSL4GlobalRenderState::alphaToCoverageEnabled) \
    { \
        GLSL4GlobalRenderState::alphaToCoverageEnabled = enabled; \
        if (enabled) \
        { \
            glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); \
        } \
        else \
        { \
            glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE); \
        } \
    }

#define APPLY_MULTISAMPLE(enabled) \
    if (enabled != GLSL4GlobalRenderState::multisampleEnabled) \
    { \
        GLSL4GlobalRenderState::multisampleEnabled = enabled; \
        if (enabled) \
        { \
            glEnable(GL_MULTISAMPLE); \
        } \
        else \
        { \
            glDisable(GL_MULTISAMPLE); \
        } \
    }

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectRenderState::GLSL4EffectRenderState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
GLSL4EffectRenderState::~GLSL4EffectRenderState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSL4EffectRenderState::Apply()
{
    // apply polygon mode
    APPLY_POLYGON_MODE(this->renderSettings.fillMode);

	// apply cull mode
    APPLY_CULL_MODE(this->renderSettings.cullMode);

	// apply stencil mode
    APPLY_STENCIL_MODE(this->renderSettings.stencilEnabled, 
        this->renderSettings.backFailOp, this->renderSettings.backDepthFailOp, this->renderSettings.backPassOp,
        this->renderSettings.frontFailOp, this->renderSettings.frontDepthFailOp, this->renderSettings.frontPassOp,
        this->renderSettings.backFaceFunc, this->renderSettings.frontFaceFunc, this->renderSettings.backRef, this->renderSettings.frontRef,
        this->renderSettings.stencilWriteMask, this->renderSettings.stencilReadMask);

    // apply blend settings
	unsigned i;
	for (i = 0; i < MaxNumRenderTargets; ++i)
	{
        APPLY_BLEND_MODE(this->renderSettings.blendEnabled[i], this->renderSettings.separateBlendEnabled, 
            this->renderSettings.srcBlends[i], this->renderSettings.dstBlends[i], this->renderSettings.alphaSrcBlends[i], this->renderSettings.alphaDstBlends[i],
            this->renderSettings.blendOps[i], this->renderSettings.alphaBlendOps[i], i);
	}

    // apply depth mode
    APPLY_DEPTH_MODE(this->renderSettings.depthEnabled, this->renderSettings.depthWriteEnabled, this->renderSettings.depthClampEnabled, this->renderSettings.depthFunction);

    // apply scissor mode
    APPLY_SCISSOR_MODE(this->renderSettings.scissorEnabled);

    // apply alpha-to-coverage
    APPLY_ALPHA_TO_COVERAGE(this->renderSettings.alphaToCoverageEnabled);

    // apply multisampling
    APPLY_MULTISAMPLE(this->renderSettings.multisampleEnabled);
}
} // namespace AnyFX