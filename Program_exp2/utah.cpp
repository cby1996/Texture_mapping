#include"stdafx.h"
#include"utah.h"
#include"draw.h"
#include"main.h"
int indices[32][4][4]={
	{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
	{4,17,18,19,8,20,21,22,12,23,24,25,16,26,27,28},
	{19,29,30,31,22,32,33,34,25,35,36,37,28,38,39,40},
	{31,41,42,1,34,43,44,5,37,45,46,9,40,47,48,13},
	{13,14,15,16,49,50,51,52,53,54,55,56,57,58,59,60},
	{16,26,27,28,52,61,62,63,56,64,65,66,60,67,68,69},
	{28,38,39,40,63,70,71,72,66,73,74,75,69,76,77,78},
	{40,47,48,13,72,79,80,49,75,81,82,53,78,83,84,57},
	{57,58,59,60,85,86,87,88,89,90,91,92,93,94,95,96},
	{60,67,68,69,88,97,98,99,92,100,101,102,96,103,104,105},
	{69,76,77,78,99,106,107,108,102,109,110,111,105,112,113,114},
	{78,83,84,57,108,115,116,85,111,117,118,89,114,119,120,93},
	{121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136},
	{124,137,138,121,128,139,140,125,132,141,142,129,136,143,144,133},
	{133,134,135,136,145,146,147,148,149,150,151,152,69,153,154,155},
	{136,143,144,133,148,156,157,145,152,158,159,149,155,160,161,69},
	{162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177},
	{165,178,179,162,169,180,181,166,173,182,183,170,177,184,185,174},
	{174,175,176,177,186,187,188,189,190,191,192,193,194,195,196,197},
	{177,184,185,174,189,198,199,186,193,200,201,190,197,202,203,194},
	{204,204,204,204,207,208,209,210,211,211,211,211,212,213,214,215},
	{204,204,204,204,210,217,218,219,211,211,211,211,215,220,221,222},
	{204,204,204,204,219,224,225,226,211,211,211,211,222,227,228,229},
	{204,204,204,204,226,230,231,207,211,211,211,211,229,232,233,212},
	{212,213,214,215,234,235,236,237,238,239,240,241,242,243,244,245},
	{215,220,221,222,237,246,247,248,241,249,250,251,245,252,253,254},
	{222,227,228,229,248,255,256,257,251,258,259,260,254,261,262,263},
	{229,232,233,212,257,264,265,234,260,266,267,238,263,268,269,242},
	{270,270,270,270,279,280,281,282,275,276,277,278,271,272,273,274},
	{270,270,270,270,282,289,290,291,278,286,287,288,274,283,284,285},
	{270,270,270,270,291,298,299,300,288,295,296,297,285,292,293,294},
	{270,270,270,270,300,305,306,279,297,303,304,275,294,301,302,271}};
GLfloat vertices_utah[306][3]={
	{1.4,0.0,2.4},{1.4,-0.784,2.4},
	{0.784,-1.4,2.4},{0.0,-1.4,2.4},{1.3375,0.0,2.53125},
	{1.3375,-0.749,2.53125},{0.749,-1.3375,2.53125},{0.0,-1.3375,2.53125},
	{1.4375,0.0,2.53125},{1.4375,-0.805,2.53125},{0.805,-1.4375,2.53125},
	{0.0,-1.4375,2.53125},{1.5,0.0,2.4},{1.5,-0.84,2.4},
	{0.84,-1.5,2.4},{0.0,-1.5,2.4},{-0.784,-1.4,2.4},
	{-1.4,-0.784,2.4},{-1.4,0.0,2.4},{-0.749,-1.3375,2.53125},
	{-1.3375,-0.749,2.53125},{-1.3375,0.0,2.53125},{-0.805,-1.4375,2.53125},
	{-1.4375,-0.805,2.53125},{-1.4375,0.0,2.53125},{-0.84,-1.5,2.4},
	{-1.5,-0.84,2.4},{-1.5,0.0,2.4},{-1.4,0.784,2.4},
	{-0.784,1.4,2.4},{0.0,1.4,2.4},{-1.3375,0.749,2.53125},
	{-0.749,1.3375,2.53125},{0.0,1.3375,2.53125},{-1.4375,0.805,2.53125},
	{-0.805,1.4375,2.53125},{0.0,1.4375,2.53125},{-1.5,0.84,2.4},
	{-0.84,1.5,2.4},{0.0,1.5,2.4},{0.784,1.4,2.4},
	{1.4,0.784,2.4},{0.749,1.3375,2.53125},{1.3375,0.749,2.53125},
	{0.805,1.4375,2.53125},{1.4375,0.805,2.53125},{0.84,1.5,2.4},
	{1.5,0.84,2.4},{1.75,0.0,1.875},{1.75,-0.98,1.875},
	{0.98,-1.75,1.875},{0.0,-1.75,1.875},{2.0,0.0,1.35},
	{2.0,-1.12,1.35},{1.12,-2.0,1.35},{0.0,-2.0,1.35},
	{2.0,0.0,0.9},{2.0,-1.12,0.9},{1.12,-2.0,0.9},
	{0.0,-2.0,0.9},{-0.98,-1.75,1.875},{-1.75,-0.98,1.875},
	{-1.75,0.0,1.875},{-1.12,-2.0,1.35},{-2.0,-1.12,1.35},
	{-2.0,0.0,1.35},{-1.12,-2.0,0.9},{-2.0,-1.12,0.9},
	{-2.0,0.0,0.9},{-1.75,0.98,1.875},{-0.98,1.75,1.875},
	{0.0,1.75,1.875},{-2.0,1.12,1.35},{-1.12,2.0,1.35},
	{0.0,2.0,1.35},{-2.0,1.12,0.9},{-1.12,2.0,0.9},
	{0.0,2.0,0.9},{0.98,1.75,1.875},{1.75,0.98,1.875},
	{1.12,2.0,1.35},{2.0,1.12,1.35},{1.12,2.0,0.9},
	{2.0,1.12,0.9},{2.0,0.0,0.45},{2.0,-1.12,0.45},
	{1.12,-2.0,0.45},{0.0,-2.0,0.45},{1.5,0.0,0.225},
	{1.5,-0.84,0.225},{0.84,-1.5,0.225},{0.0,-1.5,0.225},
	{1.5,0.0,0.15},{1.5,-0.84,0.15},{0.84,-1.5,0.15},
	{0.0,-1.5,0.15},{-1.12,-2.0,0.45},{-2.0,-1.12,0.45},
	{-2.0,0.0,0.45},{-0.84,-1.5,0.225},{-1.5,-0.84,0.225},
	{-1.5,0.0,0.225},{-0.84,-1.5,0.15},{-1.5,-0.84,0.15},
	{-1.5,0.0,0.15},{-2.0,1.12,0.45},{-1.12,2.0,0.45},
	{0.0,2.0,0.45},{-1.5,0.84,0.225},{-0.84,1.5,0.225},
	{0.0,1.5,0.225},{-1.5,0.84,0.15},{-0.84,1.5,0.15},
	{0.0,1.5,0.15},{1.12,2.0,0.45},{2.0,1.12,0.45},
	{0.84,1.5,0.225},{1.5,0.84,0.225},{0.84,1.5,0.15},
	{1.5,0.84,0.15},{-1.6,0.0,2.025},{-1.6,-0.3,2.025},
	{-1.5,-0.3,2.25},{-1.5,0.0,2.25},{-2.3,0.0,2.025},
	{-2.3,-0.3,2.025},{-2.5,-0.3,2.25},{-2.5,0.0,2.25},
	{-2.7,0.0,2.025},{-2.7,-0.3,2.025},{-3.0,-0.3,2.25},
	{-3.0,0.0,2.25},{-2.7,0.0,1.8},{-2.7,-0.3,1.8},
	{-3.0,-0.3,1.8},{-3.0,0.0,1.8},{-1.5,0.3,2.25},
	{-1.6,0.3,2.025},{-2.5,0.3,2.25},{-2.3,0.3,2.025},
	{-3.0,0.3,2.25},{-2.7,0.3,2.025},{-3.0,0.3,1.8},
	{-2.7,0.3,1.8},{-2.7,0.0,1.575},{-2.7,-0.3,1.575},
	{-3.0,-0.3,1.35},{-3.0,0.0,1.35},{-2.5,0.0,1.125},
	{-2.5,-0.3,1.125},{-2.65,-0.3,0.9375},{-2.65,0.0,0.9375},
	{-2.0,-0.3,0.9},{-1.9,-0.3,0.6},{-1.9,0.0,0.6},
	{-3.0,0.3,1.35},{-2.7,0.3,1.575},{-2.65,0.3,0.9375},
	{-2.5,0.3,1.125},{-1.9,0.3,0.6},{-2.0,0.3,0.9},
	{1.7,0.0,1.425},{1.7,-0.66,1.425},{1.7,-0.66,0.6},
	{1.7,0.0,0.6},{2.6,0.0,1.425},{2.6,-0.66,1.425},
	{3.1,-0.66,0.825},{3.1,0.0,0.825},{2.3,0.0,2.1},
	{2.3,-0.25,2.1},{2.4,-0.25,2.025},{2.4,0.0,2.025},
	{2.7,0.0,2.4},{2.7,-0.25,2.4},{3.3,-0.25,2.4},
	{3.3,0.0,2.4},{1.7,0.66,0.6},{1.7,0.66,1.425},
	{3.1,0.66,0.825},{2.6,0.66,1.425},{2.4,0.25,2.025},
	{2.3,0.25,2.1},{3.3,0.25,2.4},{2.7,0.25,2.4},
	{2.8,0.0,2.475},{2.8,-0.25,2.475},{3.525,-0.25,2.49375},
	{3.525,0.0,2.49375},{2.9,0.0,2.475},{2.9,-0.15,2.475},
	{3.45,-0.15,2.5125},{3.45,0.0,2.5125},{2.8,0.0,2.4},
	{2.8,-0.15,2.4},{3.2,-0.15,2.4},{3.2,0.0,2.4},
	{3.525,0.25,2.49375},{2.8,0.25,2.475},{3.45,0.15,2.5125},
	{2.9,0.15,2.475},{3.2,0.15,2.4},{2.8,0.15,2.4},
	{0.0,0.0,3.15},{0.0,-0.002,3.15},{0.002,0.0,3.15},
	{0.8,0.0,3.15},{0.8,-0.45,3.15},{0.45,-0.8,3.15},
	{0.0,-0.8,3.15},{0.0,0.0,2.85},{0.2,0.0,2.7},
	{0.2,-0.112,2.7},{0.112,-0.2,2.7},{0.0,-0.2,2.7},
	{-0.002,0.0,3.15},{-0.45,-0.8,3.15},{-0.8,-0.45,3.15},
	{-0.8,0.0,3.15},{-0.112,-0.2,2.7},{-0.2,-0.112,2.7},
	{-0.2,0.0,2.7},{0.0,0.002,3.15},{-0.8,0.45,3.15},
	{-0.45,0.8,3.15},{0.0,0.8,3.15},{-0.2,0.112,2.7},
	{-0.112,0.2,2.7},{0.0,0.2,2.7},{0.45,0.8,3.15},
	{0.8,0.45,3.15},{0.112,0.2,2.7},{0.2,0.112,2.7},
	{0.4,0.0,2.55},{0.4,-0.224,2.55},{0.224,-0.4,2.55},
	{0.0,-0.4,2.55},{1.3,0.0,2.55},{1.3,-0.728,2.55},
	{0.728,-1.3,2.55},{0.0,-1.3,2.55},{1.3,0.0,2.4},
	{1.3,-0.728,2.4},{0.728,-1.3,2.4},{0.0,-1.3,2.4},
	{-0.224,-0.4,2.55},{-0.4,-0.224,2.55},{-0.4,0.0,2.55},
	{-0.728,-1.3,2.55},{-1.3,-0.728,2.55},{-1.3,0.0,2.55},
	{-0.728,-1.3,2.4},{-1.3,-0.728,2.4},{-1.3,0.0,2.4},
	{-0.4,0.224,2.55},{-0.224,0.4,2.55},{0.0,0.4,2.55},
	{-1.3,0.728,2.55},{-0.728,1.3,2.55},{0.0,1.3,2.55},
	{-1.3,0.728,2.4},{-0.728,1.3,2.4},{0.0,1.3,2.4},
	{0.224,0.4,2.55},{0.4,0.224,2.55},{0.728,1.3,2.55},
	{1.3,0.728,2.55},{0.728,1.3,2.4},{1.3,0.728,2.4},
	{0.0,0.0,0.0},{1.5,0.0,0.15},{1.5,0.84,0.15},
	{0.84,1.5,0.15},{0.0,1.5,0.15},{1.5,0.0,0.075},
	{1.5,0.84,0.075},{0.84,1.5,0.075},{0.0,1.5,0.075},
	{1.425,0.0,0.0},{1.425,0.798,0.0},{0.798,1.425,0.0},
	{0.0,1.425,0.0},{-0.84,1.5,0.15},{-1.5,0.84,0.15},
	{-1.5,0.0,0.15},{-0.84,1.5,0.075},{-1.5,0.84,0.075},
	{-1.5,0.0,0.075},{-0.798,1.425,0.0},{-1.425,0.798,0.0},
	{-1.425,0.0,0.0},{-1.5,-0.84,0.15},{-0.84,-1.5,0.15},
	{0.0,-1.5,0.15},{-1.5,-0.84,0.075},{-0.84,-1.5,0.075},
	{0.0,-1.5,0.075},{-1.425,-0.798,0.0},{-0.798,-1.425,0.0},
	{0.0,-1.425,0.0},{0.84,-1.5,0.15},{1.5,-0.84,0.15},
	{0.84,-1.5,0.075},{1.5,-0.84,0.075},{0.798,-1.425,0.0},
	{1.425,-0.798,0.0}};
GLfloat data[32][4][4][3];
void UtahInit(){
	glEnable(GL_MAP2_VERTEX_3);
	int i,j,k,m,n;
	for(i=0;i<32;i++){
		for(j=0;j<4;j++){
			for(k=0;k<4;k++){
				m=indices[i][j][k];
				for(n=0;n<3;n++){
					data[i][j][k][n]=vertices_utah[m-1][n];//��ʼ����ȡ����
				}
			}
		}
	}
}
typedef float Point[3];
void normal(Point a,Point b,Point c){
	Point ab={b[0]-a[0],b[1]-a[1],b[2]-a[2]};
	Point ac={c[0]-a[0],c[1]-a[1],c[2]-a[2]};
	Point n={
		ab[1]*ac[2]-ab[2]*ac[1],
		ab[2]*ac[0]-ab[0]*ac[2],
		ab[0]*ac[1]-ab[1]*ac[0]
	};
	float l=sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
	n[0]/=l*2;n[1]/=l*2;n[2]/=l*2;
	glNormal3fv(n);
}
void divide_patch(Point p[4][4],int n);
bool drawCap;
void UtahDraw(){
	static const float MatDiffuseUtah[] = {0.8f, 0.8f, 0.6f, 1.0f};
	static const float MatSpecularUtah[] = {0.95f, 0.95f, 0.8f, 1.0f};
	DrawSetMaterial(MatDiffuseUtah,MatSpecularUtah);
	glColor3f(0.95f,0.95f,0.95f);
	glPushMatrix();
	int i;
	drawCap=false;
	for(i=0;i<32;i++){
		if(i==20)drawCap=true;
		divide_patch(data[i],5);
	}
	glFlush();
	glPopMatrix();
}
void transpose(Point a[4][4]){
	int i,j,k;
	GLfloat tt;
	for(i=0;i<4;i++) 
		for(j=i;j<4;j++) 
			for(k=0;k<3;k++){
				tt=a[i][j][k];
				a[i][j][k]=a[j][i][k];
				a[j][i][k]=tt;
			}
}
void divide_curve(Point c[4],Point r[4],Point l[4]){
	int i;
	Point t;
	for(i=0;i<3;i++){
		l[0][i]=c[0][i];
		r[3][i]=c[3][i];
		l[1][i]=(c[1][i]+c[0][i])/2;
		r[2][i]=(c[2][i]+c[3][i])/2;
		t[i]=(c[1][i]+c[2][i])/2;
		l[2][i]=(t[i]+l[1][i])/2;
		r[1][i]=(t[i]+r[2][i])/2;
		l[3][i]=r[0][i]=(l[2][i]+r[1][i])/2;
	}
}
bool line_loop=false;
void Tex(Point p){
	float c=p[0]/sqrt(p[1]*p[1]+p[0]*p[0]);
	float s=p[1]/sqrt(p[1]*p[1]+p[0]*p[0]);
	if(drawCap){
		if(!newTexture){
			glTexCoord2f((p[0]+1.5)/3,(p[1]+1.5)/3);
		}else{
			glTexCoord2f(acos(c)*(asin(s)>=0?1:-1)/3.1416f/2+(asin(s)>=0?0:1),sqrt(p[1]*p[1]+p[0]*p[0]));
		}
	}else{
		glTexCoord2f(0.5+acos(c)*(asin(s)>=0?1:-1)/3.1416f/2+(asin(s)>=0?0:1),p[2]/2.5);
	}
}
void draw_patch(Point p[4][4]){
	if(!line_loop){
		glBegin(GL_QUADS);{
			normal(p[0][0],p[3][0],p[0][3]);
			Tex(p[0][0]);glVertex3fv(p[0][0]);
			Tex(p[3][0]);glVertex3fv(p[3][0]);
			Tex(p[3][3]);glVertex3fv(p[3][3]);
			Tex(p[0][3]);glVertex3fv(p[0][3]);
		}glEnd();
	}else{
		glBegin(GL_LINE_LOOP);{
			glVertex3fv(p[0][0]);
			glVertex3fv(p[3][0]);
			glVertex3fv(p[3][3]);
			glVertex3fv(p[0][3]);
		}glEnd();
	}
}
void divide_patch(Point p[4][4],int n){
	Point q[4][4],r[4][4],s[4][4],t[4][4];
	Point a[4][4],b[4][4];
	int k;
	
	if(n==0){
		draw_patch(p);
	}
	else{
		for(k=0;k<4;k++)
			divide_curve(p[k],a[k],b[k]);
		transpose(a);
		transpose(b);
		for(k=0;k<4;k++){
			divide_curve(a[k],q[k],r[k]);
			divide_curve(b[k],s[k],t[k]);
		}
		divide_patch(q,n-1);
		divide_patch(r,n-1);
		divide_patch(s,n-1);
		divide_patch(t,n-1);
	}
}