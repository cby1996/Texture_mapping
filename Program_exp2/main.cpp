#include"stdafx.h"
#include"main.h"
#include"draw.h"

WNDPROC_DEFINE(Destroy);
WNDPROC_DEFINE(Size);
WNDPROC_DEFINE(Create);
WNDPROC_DEFINE(Mouse);
WNDPROC_DEFINE(Command);
WNDPROC_DEFINE(Key);

MMainWindow wnd;
MOpenGL gl;
WINMAIN_DEFINE{
	MWINDOWS_INIT;
	MWndClassEx wcex(MWindowGlobal::hInstance,_T("MainWnd"));
	wcex.Register();
	gl.Load(&wnd,false);
	gl.Setup(_T("exp2"),100,100,1024,768);
	wnd.Connect(WM_DESTROY,Destroy);
	wnd.Connect(WM_SIZE,Size);
	wnd.Connect(WM_CREATE,Create);
	wnd.Connect(WM_MOUSEMOVE,Mouse);
	wnd.Connect(WM_LBUTTONDOWN,Mouse);
	wnd.Connect(WM_LBUTTONUP,Mouse);
	wnd.Connect(WM_RBUTTONDOWN,Mouse);
	wnd.Connect(WM_RBUTTONUP,Mouse);
	wnd.Connect(WM_MOUSEWHEEL,Mouse);
	wnd.Connect(WM_COMMAND,Command);
	wnd.Connect(WM_KEYDOWN,Key);

	wnd.Setup();

	MWindow btn;
	btn.Create(_T("Button"),_T("透视->平行"),WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
		0,0,200,50,wnd.HWnd(),(HMENU)1,MWindowGlobal::hInstance,0);
	MWindow st;
	st.Create(_T("Static"),_T("FPS"),WS_CHILD|WS_VISIBLE|SS_CENTER,
		0,70,100,18,wnd.HWnd(),(HMENU)2,MWindowGlobal::hInstance,0);
	MWindow btn_d[3];
	btn_d[0].Create(_T("Button"),_T("桌子"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		0,100,100,25,wnd.HWnd(),(HMENU)10,MWindowGlobal::hInstance,0);
	btn_d[1].Create(_T("Button"),_T("茶壶"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		0,130,100,25,wnd.HWnd(),(HMENU)11,MWindowGlobal::hInstance,0);
	btn_d[2].Create(_T("Button"),_T("兔子"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		0,160,100,25,wnd.HWnd(),(HMENU)12,MWindowGlobal::hInstance,0);
	MWindow btn_ul;
	btn_ul.Create(_T("Button"),_T("使用DisplayList"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		0,200,200,30,wnd.HWnd(),(HMENU)3,MWindowGlobal::hInstance,0);

	wnd.Show();
	DrawInit();
	wnd.MsgLoop();
}


bool lDown=false,rDown=false,lastLDown=false,lastRDown=false;
int posX=-1,posY=-1,lastPosX=-1,lastPosY=-1;
float scale=1.0f;
float lastRotMat[]={
	1,	0,	0,	0,
	0,	1,	0,	0,
	0,	0,	1,	0,
	0,	0,	0,	1
};//rotation matrix before current draging
float curRotMat[]={
	1,	0,	0,	0,
	0,	1,	0,	0,
	0,	0,	1,	0,
	0,	0,	0,	1
};//rotation matrix of current draging
const float identityMat[]={
	1,	0,	0,	0,
	0,	1,	0,	0,
	0,	0,	1,	0,
	0,	0,	0,	1
};//be used to reset curRotMat
float rRotX0,rRotY0,rRotZ0;//begion pos of dragging sphere
float xDraft=0.0f,yDraft=0.0f;
float utahX=0.0f,utahY=0.0f,utahRot=0.0f;

bool projection=true;
int lightC=0;
bool light=false;
float lightA=10.0f;
float lightXRot=0;
float lightYRot=0;

bool newTexture=false;
inline float SphereXYZ(float &x,float &y){
	RECT rect;
	GetClientRect(wnd.HWnd(),&rect);
	float w=rect.right-rect.left,
		h=rect.bottom-rect.top;
	float midX=w/2.0f,
		midY=h/2.0f;
	x=(x-midX)/midX,y=(midY-y)/midY;
	float len2=x*x+y*y;
	if(len2>1){
		float len=sqrt(len2);
		x/=len;y/=len;
		return 0;
	}
	return sqrt(1-len2);
}

WNDPROC_DEFINE(Destroy){
	PostQuitMessage(0);
	return 0;
}
WNDPROC_DEFINE(Size){
	DrawResize();
	return WNDPROC_DEF;
}
VOID CALLBACK Timer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
WNDPROC_DEFINE(Create){
	SetTimer(hWnd,1,12,Timer);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glLineWidth(1.5);
	Draw();
	return 0;
}
WNDPROC_DEFINE(Mouse){
	if(msg==WM_MOUSEWHEEL){
		int zDelta=(short)HIWORD(wParam);
		scale+=0.0005f*zDelta;
		if(scale<0.5f){scale=0.5;}
		if(scale>3.0f){scale=3.0;}//滚轮滚动，按比例放缩
	}else{
		posX=LOWORD(lParam);
		posY=HIWORD(lParam);
		if(wParam&MK_LBUTTON){//检测左键和右键是否按下
			lDown=true;
		}else{
			lDown=false;
		}
		if(wParam&MK_RBUTTON){
			rDown=true;
		}else{
			rDown=false;
		}
	}
	return WNDPROC_DEF;
}
WNDPROC_DEFINE(Command){
	WORD wParamL=LOWORD(wParam),
		wParamH=HIWORD(wParam);
	if(lParam!=0){
		switch(wParamL){
		case 1:
			if(wParamH==BN_CLICKED){
				projection=!projection;//是否打开透视效果
				HWND btn=GetDlgItem(hWnd,1);
				if(projection){
					SetWindowText(btn,_T("透视->平行"));
				}else{
					SetWindowText(btn,_T("平行->透视"));
				}
				DrawResize();
			}
			break;
		case 3:
			useList=!useList;//设置布尔变量检测是否使用DisplayList
			if(useList){
				SetWindowText(GetDlgItem(hWnd,3),_T("使用DisplayList"));
			}else{
				SetWindowText(GetDlgItem(hWnd,3),_T("不使用DisplayList"));
			}
			break;
		case 10:case 11:case 12:
			if(wParamH==BN_CLICKED){
				draw[wParamL-10]=!draw[wParamL-10];
			}
			break;
		}
		SetFocus(hWnd);
	}
	return WNDPROC_DEF;
}
WNDPROC_DEFINE(Key){
	switch(wParam){
	case 'W':utahY+=0.1f;break;
	case 'S':utahY-=0.1f;break;
	case 'A':utahX-=0.1f;break;
	case 'D':utahX+=0.1f;break;//XY平面平移
	case 'Q':utahRot+=2.5f;break;
	case 'E':utahRot-=2.5f;break;//旋转
	case 'L':
		light=!light;break;//改变灯光效果
	case 'U':lightXRot-=1.1f;break;
	case 'J':lightXRot+=1.1f;break;
	case 'H':lightYRot-=1.1f;break;
	case 'K':lightYRot+=1.1f;break;
	case 'Y':case 'I':
		lightA+=(wParam=='Y'?1.0f:-1.0f);
		if(lightA<5)lightA=5;
		if(lightA>20)lightA=20;
		break;
	case 'O':
		++lightC;lightC%=4;break;
	case 'N':
		newTexture=!newTexture;break;//更换纹理
	}
	return 0;
}

void Refresh();
DWORD lastTick;
VOID CALLBACK Timer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){
	Refresh();
	Draw();
	DWORD curTick=GetTickCount();
	int fps=(int)(floor(1000.0f/(curTick-lastTick)));
	static TCHAR fpsBuffer[10];
	_stprintf(fpsBuffer,_T("%dFPS"),fps);
	if(fps<0){
		return;
	}
	SetWindowText(GetDlgItem(wnd.HWnd(),2),fpsBuffer);
	int tmp=GetLastError();
	lastTick=curTick;
}
float xDraft0,yDraft0;
float lamta0,sita0;
int lPosX0,lPosY0,rPosX0,rPosY0;
void LSolveDelta();
void RSolveDelta();
void Refresh(){
	if(!lastLDown&&lDown&&!lastRDown&&!rDown){//lDown
		xDraft0=xDraft;yDraft0=yDraft;
		lPosX0=posX;lPosY0=posY;
	}
	if(!lastRDown&&rDown&&!lastLDown&&!lDown){//rDown
		rRotX0=posX;rRotY0=posY;
		rRotZ0=SphereXYZ(rRotX0,rRotY0);
	}
	if(lastRDown&&!rDown&&!lastLDown&&!lDown){//rUp
		//multi cur to last
		static float tmpMat[16];
		for(int i=0;i<4;++i)for(int j=0;j<4;++j){
			float tmp=0;
			for(int k=0;k<4;++k){tmp+=curRotMat[i+k*4]*lastRotMat[k+j*4];}
			tmpMat[i+j*4]=tmp;
		}
		for(int i=0;i<16;++i){lastRotMat[i]=tmpMat[i];}
		//reset cur
		for(int i=0;i<16;++i){curRotMat[i]=identityMat[i];}
		//work out intensity (Newton's Lows said blablabla....)
	}
	if(posX!=lastPosX||posY!=lastPosY){//move
		if(lDown&&!rDown){//lDrag
			LSolveDelta();
		}
		if(!lDown&&rDown){//rDrag
			RSolveDelta();
		}
	}
	lastLDown=lDown;lastRDown=rDown;
	posX=lastPosX;posY=lastPosY;
}
void LSolveDelta(){
	RECT tmp;
	GetWindowRect(wnd.HWnd(),&tmp);
	int lenX=tmp.right-tmp.left,
		lenY=tmp.bottom-tmp.top;
	float deltaX=(float)((int)(posX)-(int)(lPosX0))/lenY*14.0f,
		deltaY=(float)((int)(posY)-(int)(lPosY0))/lenY*14.0f;
	xDraft=xDraft0+deltaX;
	if(xDraft>1.5f){xDraft=1.5f;}
	if(xDraft<-1.5f){xDraft=-1.5f;}
	yDraft=yDraft0-deltaY;
	if(yDraft>1.5f){yDraft=1.5f;}
	if(yDraft<-1.5f){yDraft=-1.5f;}
}
void RSolveDelta(){
	float x=posX,y=posY,
		z=SphereXYZ(x,y);
	/*rotate from (rRotX0,rRotY0,rRotZ0) to (x,y,z)*/
	//axis=r0Xr, lightA=r0.r
	float c=rRotX0*x+rRotY0*y+rRotZ0*z,//cos(lightA)
		s=sin(acos(c));//sin(lightA)
	float ax=rRotY0*z-rRotZ0*y,
		ay=rRotZ0*x-rRotX0*z,
		az=rRotX0*y-rRotY0*x,
		aw=sqrt(ax*ax+ay*ay+az*az);//axis
	if(aw<0.01){
		return;
	}ax/=aw;ay/=aw;az/=aw;
	curRotMat[ 0]=ax*ax*(1-c)+c;	curRotMat[ 4]=ax*ay*(1-c)-az*s;		curRotMat[ 8]=ax*az*(1-c)+ay*s;		curRotMat[12]=0;
	curRotMat[ 1]=ay*ax*(1-c)+az*s;	curRotMat[ 5]=ay*ay*(1-c)+c;		curRotMat[ 9]=ay*az*(1-c)-ax*s;		curRotMat[13]=0;
	curRotMat[ 2]=ax*az*(1-c)-ay*s;	curRotMat[ 6]=ay*az*(1-c)+ax*s;		curRotMat[10]=az*az*(1-c)+c;		curRotMat[14]=0;
	curRotMat[ 3]=0;				curRotMat[ 7]=0;					curRotMat[11]=0;					curRotMat[15]=1;
}