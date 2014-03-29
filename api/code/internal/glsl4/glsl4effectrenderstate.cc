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
	glPolygonMode(GL_FRONT_AND_BACK,
		opengl4PolygonModeTable[this->renderSettings.fillMode]);

	// apply cull mode
	if (this->renderSettings.cullMode != EffectRenderState::None)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(opengl4FaceTable[this->renderSettings.cullMode]);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	// set stencil testing
	if (this->renderSettings.stencilEnabled)
	{
		glEnable(GL_STENCIL_TEST);

		// set back and front stencil operations
		glStencilOpSeparate(	
			GL_BACK,
			opengl4StencilOpTable[this->renderSettings.backFailOp],
			opengl4StencilOpTable[this->renderSettings.backDepthFailOp],
			opengl4StencilOpTable[this->renderSettings.backPassOp]);

		glStencilOpSeparate(	
			GL_FRONT,
			opengl4StencilOpTable[this->renderSettings.frontFailOp],
			opengl4StencilOpTable[this->renderSettings.frontDepthFailOp],
			opengl4StencilOpTable[this->renderSettings.frontPassOp]);

		// set back and front stencil functions
		glStencilFuncSeparate(
			GL_BACK,
			opengl4ComparisonTable[this->renderSettings.backFaceFunc],
			this->renderSettings.backRef,
			this->renderSettings.stencilReadMask
			);

		glStencilFuncSeparate(
			GL_FRONT,
			opengl4ComparisonTable[this->renderSettings.frontFaceFunc],
			this->renderSettings.frontRef,
			this->renderSettings.stencilReadMask
			);

		// set write mask
		glStencilMask(this->renderSettings.stencilWriteMask);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}

	if (this->renderSettings.separateBlendEnabled)
	{
		unsigned i;
		for (i = 0; i < MaxNumRenderTargets; i++)
		{
			if (this->renderSettings.blendEnabled[i])
			{
				glEnablei(GL_BLEND, i);
				glBlendFuncSeparatei(
					i, 
					opengl4BlendTable[this->renderSettings.srcBlends[i]],
					opengl4BlendTable[this->renderSettings.dstBlends[i]],
					opengl4BlendTable[this->renderSettings.alphaSrcBlends[i]],
					opengl4BlendTable[this->renderSettings.alphaDstBlends[i]]
					);

				glBlendEquationSeparatei(
					i,
					opengl4EquationTable[this->renderSettings.blendOps[i]],
					opengl4EquationTable[this->renderSettings.alphaBlendOps[i]]
					);
			}
			else
			{
				glDisablei(GL_BLEND, i);
			}
		}
	}
	else
	{
		unsigned i;
		for (i = 0; i < MaxNumRenderTargets; i++)
		{
			if (this->renderSettings.blendEnabled[i])
			{
				glEnablei(GL_BLEND, i);
				glBlendFunci(
					i, 
					opengl4BlendTable[this->renderSettings.srcBlends[i]],
					opengl4BlendTable[this->renderSettings.dstBlends[i]]
					);

				glBlendEquationi(
					i,
					opengl4EquationTable[this->renderSettings.blendOps[i]]
					);
			}
			else
			{
				glDisablei(GL_BLEND, i);
			}
		}
	}

	// set depth reading
	if (this->renderSettings.depthEnabled)
	{
		glEnable(GL_DEPTH_TEST);

		// set depth writing
		glDepthMask(this->renderSettings.depthWriteEnabled ? GL_TRUE : GL_FALSE);

		// set depth function
		glDepthFunc(opengl4ComparisonTable[this->renderSettings.depthFunction]);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	// set scissor culling
	if (this->renderSettings.scissorEnabled)
	{
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}

	// enable alpha-to-coverage if
	if (this->renderSettings.alphaToCoverageEnabled)
	{
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}
	else
	{
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}

	// enable multisampling
	if (this->renderSettings.multisampleEnabled)
	{
		glEnable(GL_MULTISAMPLE);
	}
	else
	{
		glDisable(GL_MULTISAMPLE);
	}	
}
} // namespace AnyFX