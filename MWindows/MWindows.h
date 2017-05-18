// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MWINDOWS_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MWINDOWS_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef _MWINDOWS_H_
#define _MWINDOWS_H_
#pragma once

#ifdef MWINDOWS_EXPORTS
#define MWINDOWS_API __declspec(dllexport)
#else
#define MWINDOWS_API __declspec(dllimport)
#endif

/*Includes*/
#include<hash_map>
#include<tchar.h>
#pragma warning(disable:4251)
/*Class Declaration*/
class MWINDOWS_API MWindowGlobal;
class MWINDOWS_API MWndClassEx;
class MWINDOWS_API MWindow_Base;
class MWINDOWS_API MWindow;
class MWINDOWS_API MMainWindow;
/*New Windows-Style*/
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
/*Micros*/
#define MWINDOWS_INIT\
		MWindowGlobal::hInstance=hInstance;\
		MWindowGlobal::hPrevInstance=hPrevInstance;\
		MWindowGlobal::lpCmdLine=lpCmdLine;\
		MWindowGlobal::nCmdShow=nCmdShow
#define WNDPROC_DEFINE(name)\
	LRESULT WINAPI name(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
#define DLGPROC_DEFINE(name)\
	BOOL WINAPI name(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
#define WINMAIN_DEFINE\
	int WINAPI WinMain(\
	HINSTANCE hInstance,\
	HINSTANCE hPrevInstance,\
	LPSTR lpCmdLine,\
	int nCmdShow)
#define WNDPROC_DEF\
	DefWindowProc(hWnd,msg,wParam,lParam)
/*Globals*/
struct string_less:public std::binary_function<const char *,const char *,bool>{
public:
	result_type operator()(const first_argument_type& _Left, const second_argument_type& _Right)const{
		return strcmp(_Left,_Right)<0?true:false;
	}
};
template class MWINDOWS_API std::hash_map<const char *,HANDLE,std::hash_compare<const char *,string_less>>;
class MWINDOWS_API MWindowGlobal{
public:
	static HINSTANCE hInstance;
	static HINSTANCE hPrevInstance;
	static LPSTR lpCmdLine;
	static int nCmdShow;
	static std::hash_map<const char *,HANDLE,std::hash_compare<const char *,string_less>> handleHash;
private:
	MWindowGlobal(){};
};
/*Class Declarations*/
//MWindowError
#ifdef UNICODE
#define _tcscpy_s wcscpy_s
#else
#define _tcscpy_s _mbscpy_s
#endif
class MWINDOWS_API MWindowError{
private:
	static const unsigned int _errorNum=3;
	static const LPCTSTR _errorList[_errorNum];
	static const LPCTSTR _errorDescriptionList[_errorNum];
	static const unsigned int _extraDetialLength=50;
private:
	int _errorCode;
	LPCTSTR _error;
	LPCTSTR _errorDescription;
	TCHAR _extraDetial[_extraDetialLength];
public:
	MWindowError(int errorCode):
		_errorCode(errorCode),
		_error(_errorList[errorCode]),
		_errorDescription(_errorDescriptionList[errorCode])
		{_extraDetial[0]=0;};
	MWindowError(int errorCode,LPCTSTR extraDetial):
		_errorCode(errorCode),
		_error(_errorList[errorCode]),
		_errorDescription(_errorDescriptionList[errorCode])
		{_tcscpy_s(_extraDetial,_extraDetialLength,extraDetial);_extraDetial[_extraDetialLength-1]=0;};
	int Code()const{return _errorCode;};
	LPCTSTR Error()const{return _error;};
	LPCTSTR Description()const{return _errorDescription;};
	LPCTSTR ExtraDetial()const{return _extraDetial;};
};
//MWndClassEx
class MWINDOWS_API MWndClassEx{
public:
	static const UINT		defaultCbSize;
	static const UINT		defaultStyle;
	static const WNDPROC	defaultLpfnWndProc;
	static const HICON		defaultHIcon;
	static const HCURSOR	defaultHCursor;
	static const HBRUSH		defaultHbrBackground;
	static const LPCTSTR	defaultLpszClassName;
	static const HICON		defaultHIconSm;
private:
	WNDCLASSEX _wndClassEx;
	mutable bool _registered;
	mutable LPCTSTR _registeredClassName;
	mutable HINSTANCE _registeredInstance;
	static unsigned int _instanceCount;
	TCHAR _name[50];
public:
	explicit MWndClassEx(HINSTANCE hInstance,LPCTSTR className=defaultLpszClassName);
	explicit MWndClassEx(WNDCLASSEX &wndClassEx):_registered(false){_wndClassEx=wndClassEx;++_instanceCount;};
	~MWndClassEx();
	WNDCLASSEX *operator->(){return &_wndClassEx;};
	WNDCLASSEX *Get(){return &_wndClassEx;};
	const WNDCLASSEX *Get()const{return &_wndClassEx;};
	LPCTSTR Name()const{if(!_registered){throw MWindowError(0);}return _registeredClassName;}
	void Register()const;
	void Unregister()const;
};//MWndClassEx
//MWindow_Base
template class MWINDOWS_API std::hash_map<HWND,MWindow_Base *>;
template class MWINDOWS_API std::hash_map<UINT,void *>;
class MWINDOWS_API MWindow_Base{
protected:
	static std::hash_map<HWND,MWindow_Base *> _instanceMap;
protected:
	bool _exists;
	HWND _hWnd,_hParent;
	std::hash_map<UINT,void *> _procMap;
	void *_oldProc;
	bool _procMsg;
protected:
	MWindow_Base():_exists(false),_oldProc(NULL){};
	MWindow_Base(HWND hWnd,void *oldProc):_exists(true),_hWnd(hWnd),_oldProc(oldProc){_hParent=GetParent(hWnd);};
public:
	const bool Exists()const{return _exists;};
	const HWND HWnd()const{return this->_hWnd;};
	const HWND Parent()const{return this->_hParent;};
	const void *GetProc(UINT msg)const{
		std::hash_map<UINT,void *>::const_iterator hit=this->_procMap.find(msg);
		if(hit!=this->_procMap.end()){//found
			return hit->second;
		}else{
			return _oldProc;
		}
	};
	void Connect(UINT msg,void *proc){this->_procMap[msg]=proc;};
	void SetProcMsg(bool procMsg){_procMsg=procMsg;};
	bool ProcMsg(){return _procMsg;};
	virtual void Setup(bool ProcMsg=true)=0;
	void Show(){if(!_exists){throw(MWindowError(0));}ShowWindow(_hWnd,MWindowGlobal::nCmdShow);}
	void Hide(){if(!_exists){throw(MWindowError(0));}ShowWindow(_hWnd,SW_HIDE);}
protected:
	void _BindInstance(){MWindow_Base::_instanceMap[_hWnd]=(MWindow_Base *)(this);};
};
//MWindow_Base->MWindow
class MWINDOWS_API MWindow:public MWindow_Base{
protected:
	static WNDPROC_DEFINE(_MWindowProc);//LRESULT WINAPI _MWindowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
public:
	MWindow():MWindow_Base(){};
	MWindow(HWND hWnd):MWindow_Base(hWnd,(void *)(WNDPROC)GetWindowLongPtr(hWnd,GWLP_WNDPROC)){};
	HWND Create(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,
		DWORD dwStyle,int x,int y,int nWidth,int nHeight,
		HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
	HWND Create(LPCTSTR lpClassName,LPCTSTR lpWindowName,
		DWORD dwStyle,int x,int y,int nWidth,int nHeight,
		HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
	void Setup(bool ProcMsg=true);
};//MWindow
//MWindow_Base->MWindow->MMainWindow
class MWINDOWS_API MMainWindow:public MWindow{
public:
	MMainWindow():MWindow(){};
	MMainWindow(HWND hWnd):MWindow(hWnd){};
	WPARAM MsgLoop(HACCEL hAccTable=NULL);
};//MMainWindow
//MWindow_Base->MDialog
class MWINDOWS_API MDialog:public MWindow_Base{
protected:
	static BOOL CALLBACK _MDialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);
public:
	MDialog():MWindow_Base(){};
	MDialog(HWND hWnd):MWindow_Base(hWnd,(void *)(DLGPROC)GetWindowLongPtr(hWnd,DWLP_DLGPROC)){};
	HWND Create(HINSTANCE hInstance,LPCTSTR lpTemplate,HWND hWndParent,DLGPROC lpDialogFunc=NULL);
	void Setup();
};//MDialog

#endif