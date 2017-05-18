#include"stdafx.h"
#include"MWindows.h"
#include"MOpenGL.h"

/*MOpenGL*/
//statics
PIXELFORMATDESCRIPTOR MOpenGL::_defaultPfd={
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,
	16,//bits
	0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,
	PFD_MAIN_PLANE,
	0,0,0,0};
//constructors
MOpenGL::MOpenGL(MWindow *wnd,bool fullscreen):_wnd(NULL),_wcex(NULL),_hDC(NULL),_hRC(NULL),_pf(0),_pfd(NULL),_fovy(45.0){
	Load(wnd,fullscreen);
}
MOpenGL::~MOpenGL(){
	try{
		this->Remove();
	}catch(...){
	}
}
//methods
void MOpenGL::Load(MWindow *wnd,bool fullscreen){
	_wnd=wnd;
	_fullscreen=fullscreen;
	_pfd=&_defaultPfd;
	_loaded=true;
}
void MOpenGL::ResizeScene(){
	RECT tmp;
	GetWindowRect(_wnd->HWnd(),&tmp);
	this->ResizeScene(tmp.right-tmp.left,tmp.bottom-tmp.top);
}
void MOpenGL::ResizeScene(GLsizei width,GLsizei height){
	if (height==0){
		height=1;
	}
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(_fovy,(GLfloat)width/(GLfloat)height,0.1f,500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void MOpenGL::Init(){
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
}
void MOpenGL::Setup(LPCTSTR title,int x,int y,int width,int height,HWND parent,int bits){
	DWORD dwStyle=(DWORD)GetWindowLong(_wnd->HWnd(),GWL_STYLE),
		dwExStyle=(DWORD)GetWindowLong(_wnd->HWnd(),GWL_EXSTYLE);
	if(_fullscreen){
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	=width;
		dmScreenSettings.dmPelsHeight	=height;
		dmScreenSettings.dmBitsPerPel	=bits;
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		int tmp;
		if((tmp=ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN))!=DISP_CHANGE_SUCCESSFUL){
			//fullscreen failed
			_fullscreen=false;
        }
	}
	if(parent){
		dwStyle|=WS_CHILD;
	}else{
		if(_fullscreen){
			dwExStyle|=WS_EX_APPWINDOW;
			dwStyle|=WS_POPUP;
			//ShowCursor(FALSE);
		}else{
			dwExStyle|=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
			dwStyle|=WS_OVERLAPPEDWINDOW;
		}
	}
	RECT WindowRect;
	WindowRect.left=(long)0;
	WindowRect.right=(long)width;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)height;
	AdjustWindowRectEx(&WindowRect,dwStyle,FALSE,dwExStyle);
	if(!_wnd->Exists()){
		_wcex=new MWndClassEx(MWindowGlobal::hInstance,_T("MOpenglWCEx"));
		_wcex->Register();
		_wnd->Create(dwExStyle,
			_wcex->Name(),
			title,
			dwStyle|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
			x,y,width,height,parent,(HMENU)NULL,MWindowGlobal::hInstance,(LPVOID)NULL);
	}
	if(!(_hDC=GetDC(_wnd->HWnd()))){
		throw(MWindowError(1,_T("Function GetDC")));
	}
	if(!(_pf=ChoosePixelFormat(_hDC,_pfd))){
		throw(MWindowError(1,_T("Function ChoosePixelFormat")));
	}
	if(!(SetPixelFormat(_hDC,_pf,_pfd))){
		throw(MWindowError(1,_T("Function SetPixelFormat")));
	}
	if(!(_hRC=wglCreateContext(_hDC))){
		throw(MWindowError(1,_T("Function wglCreateContext")));
	}
	if(!(wglMakeCurrent(_hDC,_hRC))){
		throw(MWindowError(1,_T("Function wglMakeCurrent")));
	}
	this->ResizeScene(width,height);
}
void MOpenGL::Remove(){
	if(_fullscreen){
		ChangeDisplaySettings(NULL,0);
		//ShowCursor(TRUE);
	}
	if(_hRC){
		if(!wglMakeCurrent(NULL,NULL)){
			throw(MWindowError(1,_T("Function wglMakeCurrent")));
		}
		if(!wglDeleteContext(_hRC)){
			throw(MWindowError(1,_T("Function wglDeleteContext")));
		}
		_hRC=NULL;
	}
	if(_hDC&&(!ReleaseDC(_wnd->HWnd(),_hDC))){
		throw(MWindowError(1,_T("Function ReleaseDC")));
		_hDC=NULL;
	}
	delete _wcex;
}