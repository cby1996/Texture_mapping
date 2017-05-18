#ifndef _MOPENGL_H_
#define _MOPENGL_H_
#pragma once

#include<gl\GL.h>
#include<tchar.h>

/*Class Declaration*/
class MWINDOWS_API MOpenGL;
/*Class Defination*/
class MWINDOWS_API MOpenGL{
private:
	static PIXELFORMATDESCRIPTOR _defaultPfd;
private:
	bool _loaded;
	MWndClassEx *_wcex;
	MWindow *_wnd;
	bool _fullscreen;
	HDC _hDC;
	HGLRC _hRC;
	GLuint _pf;
	PIXELFORMATDESCRIPTOR *_pfd;
	GLdouble _fovy;
public:
	MOpenGL():_loaded(false),_wcex(NULL),_wnd(NULL),_hDC(NULL),_hRC(NULL),_pf(0),_pfd(NULL),_fovy(45.0){};
	MOpenGL(MWindow *wnd,bool fullscreen=false);
	~MOpenGL();
	void Load(MWindow *wnd,bool fullscreen=false);
	bool IsFullScreen(){return _fullscreen;};
	HDC HDC(){return _hDC;};
	HGLRC HRC(){return _hRC;};
	void ResizeScene();
	virtual void ResizeScene(GLsizei width,GLsizei height);
	virtual void Init();
	void Setup(LPCTSTR title,int x,int y,int width,int height,HWND parent=NULL,int bits=16);//
	virtual void Remove();
	virtual void Swap(){SwapBuffers(_hDC);};
	void SetFov(double fov){_fovy=fov;};
};
#endif