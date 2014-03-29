//------------------------------------------------------------------------------
//  demoapp.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "demoapp.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include "glm/gtc/matrix_transform.hpp"
#include "IL/il.h"
#include "IL/ilu.h"
#include "util.h"
#include "GL/wglew.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define clamp(x, y) x < y ? y : x

std::string simpleVs = 
"#version 400\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"layout(location = 0) in vec3 vertex;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(vertex, 1.0);\n"
"}\n";

std::string simplePs =
"#version 400\n"
"uniform vec4 faccolor;\n"
"uniform vec4 extcolor;\n"
"uniform float intensity;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = faccolor * intensity;\n"
"}\n";

std::string simplePs2 =
"#version 400\n"
"uniform vec4 faccolor;\n"
"uniform vec4 extcolor;\n"
"uniform float intensity;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = faccolor + extcolor * intensity;\n"
"}\n";

std::string simpleCs = 
"layout(rgba8) uniform writeonly image2D img;\n"
"uniform float roll;\n"
"layout(local_size_x = 16, local_size_y = 16) in;\n"
"void main()\n"
"{\n"
"ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\n"
"float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0f);\n"
"float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y) * 0.1 + roll) * 0.5f;\n"
"imageStore(img, storePos, vec4(1.0f - globalCoef, 0, 0, 0));\n"
"}\n";

std::string fuckfarts = 


"#version 430"
"//------------------------------------------------------------------------------------------------------------- "
"/**                                                                                                             "
"    pack an unsigned 16 bit value into 8 bit values, to store it into an RGBA8 texture                          "
"    input:                                                                                                      "
"        0 <= input <= 65535.0                                                                                   "
"    output:                                                                                                     "
"        0 <= byte_a < 1.0f                                                                                      "
"        0 <= byte_b < 1.0f                                                                                      "
"*/                                                                                                              "
"void pack_u16(in float depth, out float byte_a, out float byte_b)                                               "
"{                                                                                                               "
"    float tmp = depth / 256.0f;                                                                                 "
"    byte_a = floor(tmp) / 256.0f;                                                                               "
"    byte_b = frac(tmp);                                                                                         "
"                                                                                                                "
"}                                                                                                               "
"                                                                                                                "
"//------------------------------------------------------------------------------------------------------------- "
"/**                                                                                                             "
"    loot at pack_u16 for details                                                                                "
"*/                                                                                                              "
"float unpack_u16(in float byte_a, in float byte_b)                                                              "
"{                                                                                                               "
"    return ((byte_a * 256.0f) * 256.0f) + (byte_b * 256.0f);                                                    "
"}                                                                                                               "
"                                                                                                                "
"//------------------------------------------------------------------------------------------------------------- "
"/**                                                                                                             "
"    Encode a float value between -1.0 .. 1.0 into 2 seperate 8 bits. Used to store a normal component           "
"    into 2 channels of an 8-Bit RGBA texture, so the normal component is stored in 16 bits                      "
"    Normal.x -> AR 16 bit                                                                                       "
"    Normal.y -> GB 16 bit                                                                                       "
"*/                                                                                                              "
"void pack_16bit_normal_component(in float n, out float byte_a, out float byte_b)                                "
"{                                                                                                               "
"    n = ((n * 0.5f) + 0.5f) * 65535.0f;                                                                         "
"                                                                                                                "
"    pack_u16(n, byte_a, byte_b);                                                                                "
"}                                                                                                               "
"                                                                                                                "
"//------------------------------------------------------------------------------------------------------------- "
"/**                                                                                                             "
"    loot at pack_16bit_normal_component for details                                                             "
"*/                                                                                                              "
"float unpack_16bit_normal_component(in float byte_a, in float byte_b)                                           "
"{                                                                                                               "
"    return ((unpack_u16(byte_a, byte_b) / 65535.0f) - 0.5f) * 2.0f;                                             "
"}                                                                                                               "
"                                                                                                                "
"//------------------------------------------------------------------------------------------------------------- "
"/**                                                                                                             "
"    loot at pack_16bit_normal_component for details                                                             "
"*/                                                                                                              "
"vec4 pack_normalxy_into_rgba8(float normal1, float normal2)                                                     "
"{                                                                                                               "
"    vec4 ret;                                                                                                   "
"    pack_16bit_normal_component(normal1, ret.x, ret.y);                                                         "
"    pack_16bit_normal_component(normal2, ret.z, ret.w);                                                         "
"    return ret;                                                                                                 "
"}                                                                                                               "
"                                                                                                                "
"//------------------------------------------------------------------------------------------------------------- "
"/**                                                                                                             "
"    loot at pack_16bit_normal_component for details                                                             "
"*/                                                                                                              "
"vec4 unpack_normalxy_from_rgba8(vec4 packed)                                                                    "
"{                                                                                                               "
"    return vec4(  unpack_16bit_normal_component(packed.x, packed.y),                                            "
"                    unpack_16bit_normal_component(packed.z, packed.w),                                          "
"                    0.0f,                                                                                       "
"                    0.0f);                                                                                      "
"}                                                                                                               "

"void main()"
"{											"
"											"
"}											"
;

#define WINDOW_CLASS "AnyFX"
namespace AnyFX
{

DemoApp* DemoApp::instance = 0;
//------------------------------------------------------------------------------
/**
*/
DemoApp::DemoApp() :
	roll(0),
	runComputation(false),
	quit(false),
	frameIndex(0)
{
	assert(instance == 0);
	instance = this;
}

//------------------------------------------------------------------------------
/**
*/
DemoApp::~DemoApp()
{
	assert(instance != 0);
	instance = 0;
}

//------------------------------------------------------------------------------
/**
*/
DemoApp* 
DemoApp::Instance()
{
	assert(instance != 0);
	return instance;
}

//------------------------------------------------------------------------------
/**
*/
bool 
DemoApp::Open()
{
	this->width = WINDOW_WIDTH;
	this->height = WINDOW_HEIGHT;
	this->znear = 0.001f;
	this->zfar = 100000.0f;
#if __WIN32__
	ACCEL acc;
	this->hAccel = CreateAcceleratorTable(&acc, 1);
	this->hInst = GetModuleHandle(0);

	HICON icon = LoadIcon(NULL, IDI_APPLICATION);
	// register window class
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize        = sizeof(wndClass);
	wndClass.style         = CS_DBLCLKS | CS_OWNDC;
	wndClass.lpfnWndProc   = WinProc;
	wndClass.cbClsExtra    = 0;
	wndClass.cbWndExtra    = sizeof(void*);   // used to hold 'this' pointer
	wndClass.hInstance     = this->hInst;
	wndClass.hIcon         = icon;
	wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wndClass.lpszMenuName  = NULL;
	wndClass.lpszClassName = WINDOW_CLASS;
	wndClass.hIconSm       = NULL;
	RegisterClassEx(&wndClass);

	DWORD windowStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
	DWORD extendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	RECT		windowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	windowRect.left=(long)0;			// Set Left Value To 0
	windowRect.right=(long)this->width;		// Set Right Value To Requested Width
	windowRect.top=(long)0;				// Set Top Value To 0
	windowRect.bottom=(long)this->height;		// Set Bottom Value To Requested Height
	AdjustWindowRectEx(&windowRect, windowStyle, FALSE, extendedStyle);		// Adjust Window To True Requested Size

	// open window
	this->hWnd = CreateWindow(WINDOW_CLASS,
		"AnyFX Demo",					
		windowStyle,					
		0,								
		0,								
		windowRect.right-windowRect.left,						
		windowRect.bottom-windowRect.top,						
		NULL,							
		NULL,                             
		this->hInst,                      
		NULL);          

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                       //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	

	this->hDc = GetDC(this->hWnd);

	int pixelFormat = ChoosePixelFormat(this->hDc, &pfd);
	SetPixelFormat(this->hDc, pixelFormat, &pfd);
	this->hRc = wglCreateContext(this->hDc);
	wglMakeCurrent(this->hDc, this->hRc);

	ShowWindow(this->hWnd, SW_SHOW);
	
#elif __UNIX__
	this->dsp = XOpenDisplay(NULL);
	this->root = DefaultRootWindow(this->dsp);
	GLint attrs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	this->vi = glXChooseVisual(this->dsp, 0, attrs);
	this->cmap = XCreateColormap(this->dsp, this->root, vi->visual, AllocNone);
	this->swa.colormap = this->cmap;
	this->swa.event_mask = ExposureMask | KeyPressMask;
	this->win = XCreateWindow(dsp, root, 0, 0, 1024, 768, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &this->swa);
	XStoreName(this->dsp, this->win, "AnyFX Demo");
	XMapWindow(this->dsp, this->win);
	this->glc = glXCreateContext(dsp, vi, NULL, GL_TRUE);
	glXMakeCurrent(dsp, win, glc);
	this->Resize(1024, 768);
#elif __APPLE__
	CGLPixelFormatAttribute attribs[] = {(CGLPixelFormatAttribute)0};
	CGLChoosePixelFormat(attribs, &this->pix, &this->npix);
	CGLCreateContext(this->pix, NULL, &this->ctx);
	CGLSetCurrentContext(this->ctx);
#endif

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		Emit("Glew failed to initialize!\n");
	}


	printf("OpenGL specs:\n");
	printf("Vendor:   %s\n", glGetString(GL_VENDOR)); 
	printf("Renderer: %s\n", glGetString(GL_RENDERER)); 
	printf("Version:  %s\n", glGetString(GL_VERSION)); 
	printf("GLSL:     %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	this->Resize(this->width, this->height);
	// setup initial gl state
	glClearColor(0.85f, 0.85f, 1.0f, 0.5f);				
	glClearDepth(1.0f);                         // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->innerTessFactor = 1.0f;
	this->outerTessFactor = 1.0f;


	this->factory = new AnyFX::EffectFactory;
	this->effect = AnyFX::EffectFactory::Instance()->CreateEffectFromFile("../../compiled");
	this->opaqueProgram = this->effect->GetProgramByName("Solid");
	this->alphaProgram = this->effect->GetProgramByName("Alpha");
	this->tessellationProgram = this->effect->GetProgramByName("Tessellated");
	this->calcProgram = this->effect->GetProgramByName("Calculator");
	this->imgVar = this->effect->GetVariableByName("img");
	this->rollVar = this->effect->GetVariableByName("roll");
	this->viewVar = this->effect->GetVariableByName("View");
	this->projVar = this->effect->GetVariableByName("Projection");
	this->modelVar = this->effect->GetVariableByName("Model");
	this->matVar = this->effect->GetVariableByName("MaterialColor");
	this->specVar = this->effect->GetVariableByName("SpecularIntensity");
	this->texVar = this->effect->GetVariableByName("DiffuseTexture");
	this->normVar = this->effect->GetVariableByName("NormalTexture");
	this->innerTess = this->effect->GetVariableByName("TessFactorInner");
	this->outerTess = this->effect->GetVariableByName("TessFactorOuter");
	std::string mask = this->opaqueProgram->GetAnnotationString("Mask");

	this->SetupGL();


	this->model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2));

#if __WIN32__
	wglSwapIntervalEXT(-1);
#elif (__UNIX__ || __APPLE__)
	glXSwapIntervalEXT(this->dsp, glXGetCurrentDrawable(), -1);
#endif

	return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoApp::Close()
{
	//delete this->factory;
#if __WIN32__
	if (0 != this->hWnd)
	{
		DestroyWindow(this->hWnd);
		this->hWnd = 0;
	}

	// unregister the window class
	UnregisterClass(WINDOW_CLASS, this->hInst);
#else

#endif
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoApp::Run()
{
	// run loop
	while(!this->quit)
	{
		this->curTime = float(clock()) / float(CLOCKS_PER_SEC);

		// handle windows events
		this->ProcessEvents();

		float fps = 1 / (this->curTime - this->prevTime);
		std::cout << fps << std::endl;

		//GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

		// bind frame buffer
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer);
		//glDrawBuffers(2, buffers);

		// clear frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// run compute shader
		if (this->runComputation)
		{
			this->imgVar->SetTexture((void*)this->computeTextureHandle);
			this->rollVar->SetFloat(fmod(this->roll, 1024.0f));
			this->calcProgram->Apply();
			this->calcProgram->Commit();
			glDispatchCompute(512/16,512/16,1);
			//glFinish();
			//glMemoryBarrier(GL_ALL_BARRIER_BITS);
			this->roll += 0.01f;
		}

		// first, update variables
		glm::mat4 viewProj = this->view * this->projection;
		this->viewVar->SetMatrix(&this->view[0][0]);
		this->projVar->SetMatrix(&this->projection[0][0]);
		this->modelVar->SetMatrix(&this->model[0][0]);
		this->effect->GetVariableByName("ViewProjection")->SetMatrix(&viewProj[0][0]);

		this->specVar->SetFloat(1.0f);
		this->normVar->SetTexture((void*)this->diffuseTextureHandle1);
		this->texVar->SetTexture((void*)this->diffuseTextureHandle2);

		//void* test = (void*)this->diffuseTextureHandle1;
		//EffectVariable::OpenGLTexture* tex = reinterpret_cast<EffectVariable::OpenGLTexture*>(test);

		// first we apply the program, which enables shaders and render states, consider this the beginning of a pass
		this->opaqueProgram->Apply();

		// after we apply the program, we are free to batch render everything using the same program

		glBindVertexArray(this->cubeVao);

		// render cube
		glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cubeIBuffer);

		// finally, we tell AnyFX to commit all changes done to the variables, consider this as the pre-cursor to a draw call 
		// this->tessellationProgram->Commit();		

		glBindRenderbuffer(GL_RENDERBUFFER, this->depthStencilBuffer);
		unsigned i, j;
		for (i = 0; i < 1; i++)
		{
			for (j = 0; j < 1; j++)
			{
				glm::mat4 mat;
				mat = glm::translate(this->model, glm::vec3(0, i, j));
				glm::mat4 mat2;
				mat2 = mat;
				float* data = new float[32];
				memcpy((void*)data, (void*)&mat[0][0], 16 * sizeof(float));
				memcpy((void*)(data + 16), (void*)&mat2[0][0], 16 * sizeof(float));
				
				this->modelVar->SetMatrixArray(data, 2);
				this->opaqueProgram->Commit();

				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)0);

				
				this->tessellationProgram->Apply();
				this->texVar->SetTexture((void*)this->renderTarget0Handle);
				this->tessellationProgram->Commit();
				glDrawBuffer(GL_BACK);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glDrawElements(GL_PATCHES, 36, GL_UNSIGNED_INT, (GLvoid*)0);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// bind draw buffer to read buffer
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, this->framebuffer);
		
		// copy to back buffer
		//glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

#if __WIN32__
		// swap buffers to complete image
		SwapBuffers(this->hDc);
#elif __UNIX__
		glXSwapBuffers(this->dsp, this->win);
#elif __APPLE__
#endif
		//Sleep(1);
		this->frameIndex++;
		this->prevTime = this->curTime;
	}
}

#if __WIN32__
//------------------------------------------------------------------------------
/**
*/
void 
DemoApp::ProcessEvents()
{
	// don't process messages if window is null or embedded
	if (0 != this->hWnd)
	{
		assert(0 != this->hAccel);
		MSG msg;
		while (PeekMessage(&msg, this->hWnd, 0, 0, PM_REMOVE))
		{
			int msgHandled = TranslateAccelerator(this->hWnd, this->hAccel, &msg);
			if (0 == msgHandled)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
LRESULT CALLBACK
DemoApp::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DemoApp* self = DemoApp::Instance();
	switch (uMsg)
	{
	case WM_SIZE:
		self->Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND:
		// prevent Windows from erasing the background
		return 1;

	case WM_CLOSE:
		self->quit = true;
		self->hWnd = 0;
		break;

	case WM_KEYDOWN:
		if (wParam == 'W')
		{
			self->model = glm::rotate(self->model, -1.0f, glm::vec3(1, 0, 0));
		}
		else if (wParam == 'S')
		{
			self->model = glm::rotate(self->model, 1.0f, glm::vec3(1, 0, 0));
		}
		else if (wParam == 'A')
		{
			self->model = glm::rotate(self->model, -1.0f, glm::vec3(0, 1, 0));
		}
		else if (wParam == 'D')
		{
			self->model = glm::rotate(self->model, 1.0f, glm::vec3(0, 1, 0));
		}
		else if (wParam == 'Q')
		{
			self->model = glm::translate(self->model, glm::vec3(glm::inverse(self->model) * glm::vec4(0, 0, -0.1, 0)));
		}
		else if (wParam == 'E')
		{
			self->model = glm::translate(self->model, glm::vec3(glm::inverse(self->model) * glm::vec4(0, 0, 0.1, 0)));
		}
		else if (wParam == 'U')
		{
			self->innerTessFactor += 0.1f;
			self->innerTessFactor = clamp(self->innerTessFactor, 1.0f);
			self->innerTess->SetFloat(self->innerTessFactor);
		}
		else if (wParam == 'J')
		{
			self->innerTessFactor -= 0.1f;
			self->innerTessFactor = clamp(self->innerTessFactor, 1.0f);
			self->innerTess->SetFloat(self->innerTessFactor);
		}
		else if (wParam == 'I')
		{
			self->outerTessFactor += 0.1f;
			self->outerTessFactor = clamp(self->outerTessFactor, 1.0f);
			self->outerTess->SetFloat(self->outerTessFactor);
		}
		else if (wParam == 'K')
		{
			self->outerTessFactor -= 0.1f;
			self->outerTessFactor = clamp(self->outerTessFactor, 1.0f);
			self->outerTess->SetFloat(self->outerTessFactor);
		}
		else if (wParam == 'F')
		{
			self->runComputation = !self->runComputation;
		}
		else if (wParam == VK_ESCAPE)
		{
			self->quit = true;
		}

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#elif __UNIX__
//------------------------------------------------------------------------------
/**
*/
void 
DemoApp::ProcessEvents()
{
	// handle x events
	while(XPending(this->dsp))
	{

		XEvent event;
		XNextEvent(this->dsp, &event);
		if (event.type == Expose)
		{
			XGetWindowAttributes(this->dsp, this->win, &this->gwa);
		}
		else if (event.type == KeyPress)
		{
			KeySym sym = XLookupKeysym(&event.xkey, 0);
			if (sym == XK_w)
			{
				this->model = glm::rotate(this->model, -1.0f, glm::vec3(1, 0, 0));
			}
			else if (sym == XK_s)
			{
				this->model = glm::rotate(this->model, 1.0f, glm::vec3(1, 0, 0));
			}
			else if (sym == XK_a)
			{
				this->model = glm::rotate(this->model, -1.0f, glm::vec3(0, 1, 0));
			}
			else if (sym == XK_d)
			{
				this->model = glm::rotate(this->model, 1.0f, glm::vec3(0, 1, 0));
			}
			else if (sym == XK_q)
			{
				this->model = glm::translate(this->model, glm::vec3(0, 0, -0.1));
			}
			else if (sym == XK_e)
			{
				this->model = glm::translate(this->model, glm::vec3(0, 0, 0.1));
			}
			else if (sym == XK_u)
			{
				this->innerTessFactor += 0.1f;
				this->innerTessFactor = clamp(this->innerTessFactor, 1.0f);
				this->innerTess->SetFloat(this->innerTessFactor);
			}
			else if (sym == XK_j)
			{
				this->innerTessFactor -= 0.1f;
				this->innerTessFactor = clamp(this->innerTessFactor, 1.0f);
				this->innerTess->SetFloat(this->innerTessFactor);
			}
			else if (sym == XK_i)
			{
				this->outerTessFactor += 0.1f;
				this->outerTessFactor = clamp(this->outerTessFactor, 1.0f);
				this->outerTess->SetFloat(this->outerTessFactor);
			}
			else if (sym == XK_k)
			{
				this->outerTessFactor -= 0.1f;
				this->outerTessFactor = clamp(this->outerTessFactor, 1.0f);
				this->outerTess->SetFloat(this->outerTessFactor);
			}
			else if (sym == XK_f)
			{
				this->runComputation = !this->runComputation;
			}
			else if (sym == XK_Escape)
			{
				this->quit = true;
			}
		}
	}
}

#elif __APPLE__
//------------------------------------------------------------------------------
/**
*/
void 
DemoApp::ProcessEvents()
{

}
#endif

//------------------------------------------------------------------------------
/**
*/
void 
DemoApp::SetupGL()
{

	GLfloat vertices[] = 
	{
		-0.5, -0.5, 0.5, 0, 0,
		0.5, -0.5, 0.5, 1, 0,
		0.5, 0.5, 0.5, 1, 1,
		-0.5, 0.5, 0.5,	0, 1,		

		-0.5, -0.5, -0.5, 0, 0,
		0.5, -0.5, -0.5, 1, 0,
		0.5, 0.5, -0.5, 1, 1,		
		-0.5, 0.5, -0.5, 0, 1
	};

	GLuint indices[] = 
	{
		0, 1, 2,
		2, 3, 0,
		5, 4, 7,
		7, 6, 5,
		3, 2, 6,
		6, 7, 3,
		1, 5, 6,
		6, 2, 1,
		4, 0, 3,
		3, 7, 4,
		0, 4, 5,
		5, 1, 0
		//3, 7, 1,
		//5, 4, 7,
		//6, 2, 4,
		//0, 1
	};

	

	glGenTextures(1, &this->renderTarget0);
	glBindTexture(GL_TEXTURE_2D, this->renderTarget0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &this->depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->depthStencilBuffer);
	glRenderbufferStorage(
		GL_RENDERBUFFER,
		GL_DEPTH24_STENCIL8,
		this->width,
		this->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &this->framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->depthStencilBuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->renderTarget0, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	this->renderTarget0Handle = new EffectVariable::OpenGLTexture;
	this->renderTarget0Handle->texture = this->renderTarget0;
	this->renderTarget0Handle->textureType = GL_TEXTURE_2D;

	glGenBuffers(1, &this->cubeVBuffer);
	glGenBuffers(1, &this->cubeIBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cubeIBuffer);	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	

	glGenVertexArrays(1, &this->cubeVao);
	glBindVertexArray(this->cubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*) (3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	GLint compileStatus;
	GLchar log[2048];
	GLint length;

	GLint vsLength = fuckfarts.length();
	GLint psLength1 = simplePs.length();
	GLint psLength2 = simplePs2.length();
	GLint csLength = simpleCs.length();
	const GLchar* vsCode = fuckfarts.c_str();
	const GLchar* psCode1 = simplePs.c_str();
	const GLchar* psCode2 = simplePs2.c_str();
	const GLchar* csCode = simpleCs.c_str();

	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertexShader, 1, &vsCode, &vsLength);
	glCompileShader(this->vertexShader);
	glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderInfoLog(this->vertexShader, 2048, &length, log);

	this->pixelShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->pixelShader1, 1, &psCode1, &psLength1);
	glCompileShader(this->pixelShader1);
	glGetShaderiv(this->pixelShader1, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderInfoLog(this->pixelShader1, 2048, &length, log);

	this->pixelShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->pixelShader2, 1, &psCode2, &psLength2);
	glCompileShader(this->pixelShader2);
	glGetShaderiv(this->pixelShader2, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderInfoLog(this->pixelShader2, 2048, &length, log);

	this->computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(this->computeShader, 1, &csCode, &csLength);
	glCompileShader(this->computeShader);
	glGetShaderiv(this->computeShader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderInfoLog(this->computeShader, 2048, &length, log);

	this->program1 = glCreateProgram();
	glAttachShader(this->program1, this->vertexShader);
	glAttachShader(this->program1, this->pixelShader1);
	glLinkProgram(this->program1);
	glGetProgramiv(this->program1, GL_LINK_STATUS, &compileStatus);
	glGetProgramInfoLog(this->program1, 2048, &length, log);

	this->program2 = glCreateProgram();
	glAttachShader(this->program2, this->vertexShader);
	glAttachShader(this->program2, this->pixelShader2);
	glLinkProgram(this->program2);
	glGetProgramiv(this->program2, GL_LINK_STATUS, &compileStatus);
	glGetProgramInfoLog(this->program2, 2048, &length, log);

	this->program3 = glCreateProgram();
	glAttachShader(this->program3, this->computeShader);
	glLinkProgram(this->program3);
	glGetProgramiv(this->program3, GL_LINK_STATUS, &compileStatus);
	glGetProgramInfoLog(this->program3, 2048, &length, log);

	this->projectionVariable = glGetUniformLocation(this->program1, "projection");
	this->viewVariable = glGetUniformLocation(this->program1, "view");
	this->modelVariable = glGetUniformLocation(this->program1, "model");	
	this->computeRollVar = glGetUniformLocation(this->program3, "roll");
	this->computeImgVar = glGetUniformLocation(this->program3, "img");

	GLint p1Color = glGetUniformLocation(this->program1, "faccolor");
	GLint p1ExtColor = glGetUniformLocation(this->program1, "extcolor");
	GLint p1Intensity = glGetUniformLocation(this->program1, "intensity");

	GLint p2Color = glGetUniformLocation(this->program2, "faccolor");
	GLint p2ExtColor = glGetUniformLocation(this->program2, "extcolor");
	GLint p2Intensity = glGetUniformLocation(this->program2, "intensity");

	//glProgramUniformMatrix4fv(this->program1, this->projectionVariable, 1, GL_FALSE, &projection[0][0]);

	ilInit();

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoad(IL_JPG, "../../image.jpg");
	iluFlipImage();
	ILenum error = ilGetError();

	ILuint imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
	ILuint imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	ILubyte* data = ilGetData();

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	ilBindImage(0);
	ilDeleteImage(image);

	image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoad(IL_JPG, "../../mask.jpg");
	iluFlipImage();
	error = ilGetError();

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	data = ilGetData();

	glGenTextures(1, &this->maskTexture);
	glBindTexture(GL_TEXTURE_2D, this->maskTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	ilBindImage(0);
	ilDeleteImage(image);

	glGenTextures(1, &this->computeTexture);
	glBindTexture(GL_TEXTURE_2D, computeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->diffuseTextureHandle1 = new EffectVariable::OpenGLTexture;
	this->diffuseTextureHandle1->textureType = GL_TEXTURE_2D;
	this->diffuseTextureHandle1->texture = this->texture;

	this->diffuseTextureHandle2 = new EffectVariable::OpenGLTexture;
	this->diffuseTextureHandle2->textureType = GL_TEXTURE_2D;
	this->diffuseTextureHandle2->texture = this->maskTexture;

	this->computeTextureHandle = new EffectVariable::OpenGLTexture;
	this->computeTextureHandle->textureType = GL_TEXTURE_2D;
	this->computeTextureHandle->texture = this->computeTexture;
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoApp::Resize( int width, int height )
{
	glViewport(0, 0, this->width, this->height);							

	this->width = width;
	this->height = height;

	this->projection = glm::perspective(60.0f, this->width/float(this->height), this->znear, this->zfar);
	this->view = glm::mat4(1.0f);
}
} // namespace AnyFX
