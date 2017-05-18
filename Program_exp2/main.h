#ifndef _MAIN_H_
#define _MAIN_H_
#pragma once

extern MMainWindow wnd;
extern MOpenGL gl;

extern float scale;
extern float xDraft;
extern float yDraft;
extern bool projection;
extern float curRotMat[16],lastRotMat[16];
extern float utahX,utahY,utahRot;
extern bool light;
extern float lightA;
extern int lightC;
extern float lightXRot;
extern float lightYRot;

extern bool newTexture;

#endif