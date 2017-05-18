#ifndef _DRAW_H_
#define _DRAW_H_
#pragma once

void DrawInit();
void DrawResize();
void Draw();
void DrawSetMaterial(const float *diffuse,const float *specular,int shininess=50);
extern bool draw[3];
extern bool useList;

#endif