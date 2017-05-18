#include"stdafx.h"
#include"draw.h"
#include"main.h"
#include"utah.h"
#include"stanford_bunny.h"
#include"texture.h"

bool draw[3]={true,true,true};
bool useList=true;
GLuint lists;

void DrawTable();
void DrawUtah(bool nt);
void DrawBunny();
void DrawAxis();
void DrawCameraX();
	
const GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat LightDiffuse[]= { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat LightPosition[]= { 0.0f, 0.0f, 10.0f, 0.0f };
const GLfloat LightAmbient1[][4]={{0.2f,0.2f,0.2f,1.0f},{0.6f,0.0f,0.0f,1.0f},{0.0f,0.6f,0.0f,1.0f},{0.0f,0.0f,0.6f,1.0f}};
const GLfloat LightSpecular1[][4]={{3.0f,3.0f,3.0f,1.0f},{9.0f,0.0f,0.0f,1.0f},{0.0f,9.0f,0.0f,1.0f},{0.0f,0.0f,9.0f,1.0f}};
const GLfloat LightDiffuse1[][4]={{2.0f,2.0f,2.0f,1.0f},{6.0f,0.0f,0.0f,1.0f},{0.0f,6.0f,0.0f,1.0f},{0.0f,0.0f,6.0f,1.0f}};
const GLfloat position1[4]={0.0f,0.0f,6.0f,1.0f};
const GLfloat direction1[4]={0.0f,0.0f,-1.0f,1.0f};//
GLuint texture[4];
void DrawInit(){
	//
	glEnable(GL_DEPTH_TEST);//
	glEnable(GL_NORMALIZE);
	//light
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	//glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT0);
	//texture
	glGenTextures(4,texture);
	texload(texture[0],"Crack.bmp");
	//texload(texture[0],"scene.bmp")
	//texload(texture[1],"Monet.bmp");
	texload(texture[1], "cr7.bmp");
	texload(texture[2],"Spot.bmp");
	texload(texture[3],"star.bmp");//读取位图文件
	//display list
	lists=glGenLists(6);//创建6个连续的List id
	glNewList(lists+0,GL_COMPILE);{//axis
		DrawAxis();
	}glEndList();
	glNewList(lists+1,GL_COMPILE);{//camera cross
		DrawCameraX();
	}glEndList();
	glNewList(lists+2,GL_COMPILE);{//table
		DrawTable();
	}glEndList();
	UtahInit();
	glNewList(lists+3,GL_COMPILE);{//utah
		DrawUtah(false);
	}glEndList();
	glNewList(lists+5,GL_COMPILE);{//utah new
		DrawUtah(true);
	}glEndList();
	glNewList(lists+4,GL_COMPILE);{//rabbit
		DrawBunny();
	}glEndList();
	DrawResize();
}
void DrawResize(){
	RECT rect;
	GetClientRect(wnd.HWnd(),&rect);
	float w=rect.right-rect.left;
	float h=rect.bottom-rect.top;
	if(h==0){h=1;}
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(projection){
		gluPerspective(45,w/h,0.1f,100);
	}else{
		glOrtho(-6*w/h,6*w/h,-6,6,0.1f,100);
	}
	glMatrixMode(GL_MODELVIEW);
}
void Draw(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-15.0f);
	glTranslatef(xDraft,yDraft,0.0f);
	glMultMatrixf(curRotMat);glMultMatrixf(lastRotMat);
	glScalef(scale,scale,scale);
	if(light){
		glPushMatrix();
		glRotatef(lightXRot,1,0,0);
		glRotatef(lightYRot,0,1,0);
		glLightfv(GL_LIGHT1, GL_POSITION,position1);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,direction1);          //光源方向
		glLightfv(GL_LIGHT1, GL_AMBIENT,LightAmbient1[lightC]);             //设置环境光成分
		glLightfv(GL_LIGHT1, GL_SPECULAR,LightSpecular1[lightC]);                    //设置镜面光成分
		glLightfv(GL_LIGHT1, GL_DIFFUSE,LightDiffuse1[lightC]);                     //设置漫射光成分
		glLightf(GL_LIGHT1,  GL_SPOT_CUTOFF,lightA);             //裁减角度
		glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,2.0f);                    //聚集度
		glEnable(GL_LIGHT1);
		glPopMatrix();
	}else{
		glDisable(GL_LIGHT1);
	}
	if(draw[0]){//table
		if(useList)glCallList(lists+2);
		else DrawTable();
	}
	if(draw[1]){//utah
		glPushMatrix();
		glTranslatef(utahX-1,utahY,0.5f);
		glRotatef(utahRot,0,0,1);
		glScalef(0.3f,0.3f,0.3f);
		if(useList){
			if(!newTexture){
				glCallList(lists+3);
			}else{
				glCallList(lists+5);
			}
		}
		else DrawUtah(newTexture);
		glPopMatrix();
	}
	if(draw[2]){//rabbit
		glPushMatrix();
		glTranslatef(utahX+1,utahY,1);
		glRotatef(utahRot,0,0,1);
		glRotatef(90,1,0,0);
		glScalef(1,1,1);
		if(useList)glCallList(lists+4);
		else DrawBunny();
		glPopMatrix();
	}
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-6,6,-6,6,1,100);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-4,0,-15);
	glMultMatrixf(curRotMat);glMultMatrixf(lastRotMat);
	glScalef(scale,scale,scale);
	if(useList)glCallList(lists+0);//axis
	else DrawAxis();
	glLoadIdentity();
	glEnable(GL_POINT_SMOOTH);
	if(useList)glCallList(lists+1);//camera cross
	else DrawCameraX();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	gl.Swap();
}

void DrawCube(float x,float y,float z,float lx,float ly,float lz);
void DrawTable(){
	//static const float MatDiffuseDesktop[4] = {0.9f,0.7f,0.5f,1.0f};
	//static const float MatSpecularDesktop[4] = {0.6f, 0.6f, 0.6f, 1.0f};
	//static const float MatDiffuseDeskLeg[4] ={0.8f,0.6f,0.4f,1.0f};
	//static const float MatSpecularDeskLeg[4] = {0.6f, 0.6f, 0.6f, 1.0f};
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glColor4f(0.9f,0.7f,0.5f,0.0f);
	//DrawSetMaterial(MatDiffuseDesktop,MatSpecularDesktop);
	DrawCube(0,0,0,5,4,1);//0
	glColor4f(0.8f,0.6f,0.4f,0.0f);
	//DrawSetMaterial(MatDiffuseDeskLeg,MatSpecularDeskLeg);
	DrawCube(+1.5,+1,-2,0.5f,0.5f,3);//1
	DrawCube(+1.5,-1,-2,0.5f,0.5f,3);//2
	DrawCube(-1.5,-1,-2,0.5f,0.5f,3);//3
	DrawCube(-1.5,+1,-2,0.5f,0.5f,3);//4
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glScalef(1.001f,1.001f,1.001f);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	//glColor3f(0.9f,0.7f,0.5f);
	//DrawSetMaterial(MatDiffuseDesktop,MatSpecularDesktop);
	DrawCube(0,0,0,5,4,1);//0
	//glColor3f(0.8f,0.6f,0.4f);
	//DrawSetMaterial(MatDiffuseDeskLeg,MatSpecularDeskLeg);
	DrawCube(+1.5,+1,-2,0.5f,0.5f,3);//1
	DrawCube(+1.5,-1,-2,0.5f,0.5f,3);//2
	DrawCube(-1.5,-1,-2,0.5f,0.5f,3);//3
	DrawCube(-1.5,+1,-2,0.5f,0.5f,3);//4
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void DrawUtah(bool nt){
	
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_ADD);
	if(!nt){
		glBindTexture(GL_TEXTURE_2D,texture[1]);
	}else{
		glBindTexture(GL_TEXTURE_2D,texture[3]);
	}
	UtahDraw();
	glDisable(GL_TEXTURE_2D);
}
void DrawBunny(){
	bunny_draw();
}
void DrawAxis(){
	glBegin(GL_LINES);{
		glColor3f(0.2f,0.4f,0.8f);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,0,1);
	}glEnd();
}
void DrawCameraX(){
	glPointSize(20);
	glBegin(GL_POINTS);{
		glColor3f(1,1,1);
		glVertex3f(0,0,-15);
	}glEnd();
	glBegin(GL_LINES);{
		glColor3f(0,0,0);
		glVertex3f(+0.1f,0,-15);
		glVertex3f(-0.1f,0,-15);
		glVertex3f(0,+0.1f,-15);
		glVertex3f(0,-0.1f,-15);
	}glEnd();
}
void DrawCube(float x,float y,float z,float lx,float ly,float lz){
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(lx,ly,lz);
	float r=0.5f;
	glBegin(GL_QUADS);{
		//right
		glNormal3d(+1.0f,0,0);
		glTexCoord2f(0,0);glVertex3f(+r,+r,+r);
		glTexCoord2f(0,1);glVertex3f(+r,+r,-r);
		glTexCoord2f(1,1);glVertex3f(+r,-r,-r);
		glTexCoord2f(1,0);glVertex3f(+r,-r,+r);
		//left
		glNormal3d(-1.0f,0,0);
		glTexCoord2f(0,1);glVertex3f(-r,+r,+r);
		glTexCoord2f(0,0);glVertex3f(-r,+r,-r);
		glTexCoord2f(1,0);glVertex3f(-r,-r,-r);
		glTexCoord2f(1,1);glVertex3f(-r,-r,+r);
		//top
		glNormal3d(0,+1.0f,0);
		glTexCoord2f(1,1);glVertex3f(+r,+r,+r);
		glTexCoord2f(1,0);glVertex3f(+r,+r,-r);
		glTexCoord2f(0,0);glVertex3f(-r,+r,-r);
		glTexCoord2f(0,1);glVertex3f(-r,+r,+r);
		//bottom
		glNormal3d(0,-1.0f,0);
		glTexCoord2f(1,1);glVertex3f(+r,-r,+r);
		glTexCoord2f(1,0);glVertex3f(+r,-r,-r);
		glTexCoord2f(0,0);glVertex3f(-r,-r,-r);
		glTexCoord2f(0,1);glVertex3f(-r,-r,+r);
		//front
		glNormal3d(0,0,+1.0f);
		glTexCoord2f(1,1);glVertex3f(+r,+r,+r);
		glTexCoord2f(1,0);glVertex3f(+r,-r,+r);
		glTexCoord2f(0,0);glVertex3f(-r,-r,+r);
		glTexCoord2f(0,1);glVertex3f(-r,+r,+r);
		//back
		glNormal3d(0,0,-1.0f);
		glTexCoord2f(1,1);glVertex3f(+r,+r,-r);
		glTexCoord2f(1,0);glVertex3f(+r,-r,-r);
		glTexCoord2f(0,0);glVertex3f(-r,-r,-r);
		glTexCoord2f(0,1);glVertex3f(-r,+r,-r);
	}glEnd();
	glPopMatrix();
}


void DrawSetMaterial(const float *diffuse,const float *specular,int shininess){
    glMaterialfv(GL_FRONT, GL_DIFFUSE,diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,specular);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
}