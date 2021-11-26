#pragma once
#include <Windows.h>

namespace com {

typedef bool(APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int interval);
typedef int(APIENTRY *PFNWGLGETSWAPINTERVALEXTPROC)(void);

class GLContext {
	HDC		hdc_;
	HGLRC	hrc_;
	HWND	hwnd_;
	unsigned int pixelFormat_;
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT_ = nullptr;
	PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT_ = nullptr;
public:
	GLContext(HWND hwnd);
	GLContext(const GLContext &) = delete;
	GLContext &operator=(const GLContext &) = delete;
	void releaseCurrent();
	void makeCurrent();
	HDC getHDC() const;
	HGLRC getHGLRC() const;
	void swapBuffers();
	bool initialize();
	~GLContext();
};

}