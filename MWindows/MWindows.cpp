//MWindows.cpp : 定义 DLL 应用程序的导出函数。
//

#include"stdafx.h"
#include"MWindows.h"

/*Globals*/
HINSTANCE MWindowGlobal::hInstance=NULL;
HINSTANCE MWindowGlobal::hPrevInstance=NULL;
LPSTR MWindowGlobal::lpCmdLine=NULL;
int MWindowGlobal::nCmdShow=0;
std::hash_map<const char *,HANDLE,std::hash_compare<const char *,string_less>> MWindowGlobal::handleHash;
/*MWindowError*/
//static consts
const LPCTSTR MWindowError::_errorList[_errorNum]={
/*0*/	_T("Wrong Method"),
/*1*/	_T("Failed to Call WinAPI"),
/*2*/	_T(""),
};
const LPCTSTR MWindowError::_errorDescriptionList[_errorNum]={
/*0*/	_T("You cannot call this method now. Please check the current state of the class."),
/*1*/	_T("WinAPI function failed. To get extended error information, call GetLastError"),
/*2*/	_T(""),
};
/*MWndClassEx*/
//static consts
unsigned int MWndClassEx::_instanceCount=0;
const UINT		MWndClassEx::defaultCbSize=sizeof(WNDCLASSEX);
const UINT		MWndClassEx::defaultStyle=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
const WNDPROC	MWndClassEx::defaultLpfnWndProc=DefWindowProc;
const HICON		MWndClassEx::defaultHIcon=LoadIcon(NULL,IDI_APPLICATION);
const HCURSOR	MWndClassEx::defaultHCursor=LoadCursor(NULL,IDC_ARROW);
const HBRUSH	MWndClassEx::defaultHbrBackground=(HBRUSH)(COLOR_WINDOW+1);
const LPCTSTR	MWndClassEx::defaultLpszClassName=_T("defaultMWndClassEx");
const HICON		MWndClassEx::defaultHIconSm=LoadIcon(NULL,IDI_APPLICATION);
//constuctors
MWndClassEx::MWndClassEx(HINSTANCE hInstance,LPCTSTR className):_registered(false){
	++_instanceCount;
	_stprintf_s<50>(_name,_T("%s_%d\0"),className,_instanceCount);
	_wndClassEx.cbSize			=defaultCbSize;
	_wndClassEx.style			=defaultStyle;
	_wndClassEx.lpfnWndProc		=defaultLpfnWndProc;
	_wndClassEx.cbClsExtra		=0;
	_wndClassEx.cbWndExtra		=0;
	_wndClassEx.hInstance		=hInstance;
	_wndClassEx.hIcon			=defaultHIcon;
	_wndClassEx.hCursor			=defaultHCursor;
	_wndClassEx.hbrBackground	=defaultHbrBackground;
	_wndClassEx.lpszMenuName	=NULL;
	_wndClassEx.lpszClassName	=_name;
	_wndClassEx.hIconSm			=defaultHIconSm;
};
MWndClassEx::~MWndClassEx(){
	--_instanceCount;
	try{
		this->Unregister();
	}catch(...){
	}
}
//methods
void MWndClassEx::Register()const{
	if(_registered){
		throw(MWindowError(0));
	}
	if(!RegisterClassEx(&_wndClassEx)){
		throw(MWindowError(1,_T("Function RegisterClassEx")));
	}else{
		_registeredClassName=_wndClassEx.lpszClassName;
		_registeredInstance=_wndClassEx.hInstance;
		_registered=true;
	}
}
void MWndClassEx::Unregister()const{
	if(!_registered){
		throw(MWindowError(0));
	}else{
		if(!UnregisterClass(_registeredClassName,_registeredInstance)){
			throw(MWindowError(1,_T("Function UnregisterClass")));
		}else{
			_registered=false;
		}
	}
}
/*MWindow_Base*/
std::hash_map<HWND,MWindow_Base *> MWindow_Base::_instanceMap;
/*MWindow_Base->MWindow*/
//statics
LRESULT WINAPI MWindow::_MWindowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	std::hash_map<HWND,MWindow_Base *>::const_iterator hit=_instanceMap.find(hWnd);
	if(hit!=_instanceMap.end()){//found
		if(!(hit->second->ProcMsg())){
			SendMessage(hit->second->Parent(),msg,wParam,lParam);
		}
		const void *tmp=hit->second->GetProc(msg);
		if(tmp){
			return ((WNDPROC)(tmp))(hWnd,msg,wParam,lParam);
		}
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
//methods
HWND MWindow::Create(DWORD dwExStyle,LPCWSTR lpClassName,LPCWSTR lpWindowName,
	DWORD dwStyle,int x,int y,int nWidth,int nHeight,
	HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam){
		WNDCLASSEX tmp;
		GetClassInfoEx(hInstance,lpClassName,&tmp);
		_hWnd=CreateWindowEx(dwExStyle,lpClassName,lpWindowName,
			dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
		if(!_hWnd){
			throw(MWindowError(1,_T("Function CreateWindowEx")));
		}
		_hParent=hWndParent;
		_exists=true;
		return _hWnd;
}
HWND MWindow::Create(LPCWSTR lpClassName,LPCWSTR lpWindowName,
	DWORD dwStyle,int x,int y,int nWidth,int nHeight,
	HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam){
		_hWnd=CreateWindow(lpClassName,lpWindowName,
			dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
		if(!_hWnd){
			int tmp=GetLastError();
			throw(MWindowError(1,_T("Function CreateWindow")));
		}
		_hParent=hWndParent;
		_exists=true;
		return _hWnd;
}
void MWindow::Setup(bool procMsg){
	if(!_exists){
		throw(MWindowError(0));
	}else{
		_procMsg=procMsg;
	int err;
	err=GetLastError();
		_oldProc=(WNDPROC)SetWindowLongPtr(_hWnd,GWLP_WNDPROC,(LONG)(MWindow::_MWindowProc));
	err=GetLastError();
		this->_BindInstance();
		if(procMsg){
			SendMessage(_hWnd,WM_CREATE,0,0);
	err=GetLastError();
		}
	}
}
/*MWindow_Base->MWindow->MMainWindow*/
//methods
WPARAM MMainWindow::MsgLoop(HACCEL hAccTable){
	if(!_exists){
		throw(MWindowError(0));
	}else{
		MSG msg;
		BOOL fGotMsg;
		while((fGotMsg=GetMessage(&msg,(HWND)NULL,0,0))>0){
			if(!hAccTable||!TranslateAccelerator(_hWnd,hAccTable,&msg)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return msg.wParam;
	}
}
/*MWindow_Base->MDialog*/
//static
BOOL CALLBACK MDialog::_MDialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam){
	std::hash_map<HWND,MWindow_Base *>::const_iterator hit=_instanceMap.find(hDlg);
	if(hit!=_instanceMap.end()){//found
		if(!(hit->second->ProcMsg())){
			SendMessage(hit->second->Parent(),msg,wParam,lParam);
		}
		const void *tmp=hit->second->GetProc(msg);
		if(tmp){
			return ((DLGPROC)(tmp))(hDlg,msg,wParam,lParam);
		}
	}
	return false;
}
//methods
HWND MDialog::Create(HINSTANCE hInstance,LPCTSTR lpTemplate,HWND hWndParent,DLGPROC lpDialogFunc){
	_hWnd=CreateDialog(hInstance,lpTemplate,hWndParent,lpDialogFunc);
	if(!_hWnd){
		throw(MWindowError(1,_T("Function CreateDialog")));
	}
	_hParent=hWndParent;
	_exists=true;
	return _hWnd;
}
void MDialog::Setup(){
	if(!_exists){
		throw(MWindowError(0));
		return;
	}else{
		_oldProc=(void *)(DLGPROC)SetWindowLongPtr(_hWnd,DWLP_DLGPROC,(LONG)(MDialog::_MDialogProc));
		this->_BindInstance();
		SendMessage(_hWnd,WM_INITDIALOG,0,0);
	}
}