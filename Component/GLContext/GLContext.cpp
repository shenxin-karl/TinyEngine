#include "GLContext.h"
#include <glad/glad.h>
#include <gl/GL.h>
#include <dwmapi.h>
#include <iostream>

namespace com {

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

#if defined(__GNUC__)
// Workaround GCC warning from -Wcast-function-type.
#define wglGetProcAddress (void *)wglGetProcAddress
#endif

typedef HGLRC(APIENTRY *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int *);

GLContext::GLContext(HWND hwnd) : hdc_(nullptr), hrc_(nullptr), hwnd_(hwnd) {
	if (!initialize()) {
		std::cerr << "Failed initialize GLContext" << std::endl;
		throw -1;
	}
}

void GLContext::releaseCurrent() {
	wglMakeCurrent(hdc_, NULL);
}

void GLContext::makeCurrent() {
	wglMakeCurrent(hdc_, hrc_);
}

HDC GLContext::getHDC() const {
	return hdc_;
}

HGLRC GLContext::getHGLRC() const {
	return hrc_;
}

void GLContext::swapBuffers() {
	SwapBuffers(hdc_);
}

bool GLContext::initialize() {
	static PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
		1,
		PFD_DRAW_TO_WINDOW | // Format Must Support Window
		PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,
		(BYTE)PFD_TYPE_RGBA,
		(BYTE)32,
		(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, // Color Bits Ignored
		(BYTE)8 , // Alpha Buffer
		(BYTE)0, // Shift Bit Ignored
		(BYTE)0, // No Accumulation Buffer
		(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, // Accumulation Bits Ignored
		(BYTE)24, // 24Bit Z-Buffer (Depth Buffer)
		(BYTE)0, // No Stencil Buffer
		(BYTE)0, // No Auxiliary Buffer
		(BYTE)PFD_MAIN_PLANE, // Main Drawing Layer
		(BYTE)0, // Reserved
		0, 0, 0 // Layer Masks Ignored
	};

	hdc_ = GetDC(hwnd_);
	if (!hdc_)
		return false; 

	pixelFormat_ = ChoosePixelFormat(hdc_, &pfd);
	if (!pixelFormat_) // Did Windows Find A Matching Pixel Format?
		return false; // Return FALSE

	BOOL ret = SetPixelFormat(hdc_, pixelFormat_, &pfd);
	if (!ret) // Are We Able To Set The Pixel Format?
		return false; // Return FALSE

	hrc_ = wglCreateContext(hdc_);
	if (!hrc_) // Are We Able To Get A Rendering Context?
		return false; // Return FALSE

	wglMakeCurrent(hdc_, hrc_);
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, //we want a 3.3 context
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		//and it shall be forward compatible so that we can only use up to date functionality
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB /*| _WGL_CONTEXT_DEBUG_BIT_ARB*/,
		0
	}; //zero indicates the end of the array

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL; //pointer to the method
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB == NULL) { //OpenGL 3.0 is not supported
		wglDeleteContext(hrc_);
		return false;
	}

	HGLRC new_hRC = wglCreateContextAttribsARB(hdc_, 0, attribs);
	if (!new_hRC) {
		wglDeleteContext(hrc_);
		return false; // Return false
	}
	wglMakeCurrent(hdc_, NULL);
	wglDeleteContext(hrc_);
	hrc_ = new_hRC;

	if (!wglMakeCurrent(hdc_, hrc_)) // Try To Activate The Rendering Context
		return false; // Return FALSE

	wglSwapIntervalEXT_ = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapIntervalEXT_ = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	//glWrapperInit(wrapper_get_proc_address);
	return true;
}

GLContext::~GLContext() {
	releaseCurrent();
}

}