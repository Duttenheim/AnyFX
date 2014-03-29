#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::DemoApp
    
    Implements an application which is used to demonstrate AnyFX
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "GL/glew.h"
#include <time.h>

#if __WIN32__
#include <Windows.h>
#include <WinDef.h>
#elif __UNIX__
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glxew.h>
#elif __APPLE__
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/OpenGL.h>
#endif
#include "glm/glm.hpp"
#include "afxapi.h"

namespace AnyFX
{
class DemoApp
{
public:
	/// constructor
	DemoApp();
	/// destructor
	virtual ~DemoApp();

	/// opens the app
	bool Open();
	/// closes the app
	void Close();
	/// runs the app
	void Run();

	/// returns instance to app
	static DemoApp* Instance();

private:

	/// setup geometry meshes
	void SetupGL();
	/// process events
	void ProcessEvents();

	/// resizes window
	void Resize(int width, int height);

#if __WIN32__
	/// windows event handler loop
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HGLRC hRc;
	HACCEL hAccel;
	HINSTANCE hInst;
	HWND hWnd;
	HDC hDc;
#elif __UNIX__
	Display* dsp;
	Window root;
	Window win;
	XVisualInfo* vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	GLXContext glc;
	XWindowAttributes gwa;
#elif __APPLE__
	CGLContextObj ctx;
	CGLPixelFormatObj pix;
	GLint npix;
#endif

	int width;
	int height;
	float zfar;
	float znear;

	float yRot;
	float xRot;

	GLuint vertexShader;
	GLuint pixelShader1;
	GLuint pixelShader2;
	GLuint computeShader;
	GLuint program1;
	GLuint program2;
	GLuint program3;

	GLint projectionVariable;
	GLint viewVariable;
	GLint modelVariable;
	GLint colorVar;
	GLint extColorVar;
	GLint intensityVar;
	GLint computeRollVar;
	GLint computeImgVar;

	GLuint framebuffer;
	GLuint depthStencilBuffer;
	GLuint renderTarget0;
	EffectVariable::OpenGLTexture* renderTarget0Handle;

	GLuint cubeVao;
	GLuint cubeVBuffer;
	GLuint cubeIBuffer;
	GLuint texture;
	GLuint maskTexture;
	GLuint computeTexture;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	bool quit;

	static DemoApp* instance;

	EffectFactory* factory;
	Effect* effect;
	EffectProgram* opaqueProgram;
	EffectProgram* alphaProgram;
	EffectProgram* tessellationProgram;
	EffectProgram* calcProgram;
	EffectVariable* imgVar;
	EffectVariable* rollVar;
	EffectVariable* projVar;
	EffectVariable* viewVar;
	EffectVariable* modelVar;
	EffectVariable* matVar;
	EffectVariable* specVar;
	EffectVariable* texVar;
	EffectVariable* normVar;
	EffectVariable* innerTess;
	EffectVariable* outerTess;

	EffectVariable::OpenGLTexture* diffuseTextureHandle1;
	EffectVariable::OpenGLTexture* diffuseTextureHandle2;
	EffectVariable::OpenGLTexture* computeTextureHandle;

	float innerTessFactor;
	float outerTessFactor;
	float roll;
	bool runComputation;
	unsigned frameIndex;

	float prevTime;
	float curTime;

}; 
} // namespace AnyFX
//------------------------------------------------------------------------------
