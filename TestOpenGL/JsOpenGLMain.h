//
//  JsOpenGLMain.h
//  TestOpenGL
//
//  Created by Jozef Skrabo on 23/06/12.
//  Copyright (c) 2012 Jozef Skrabo. All rights reserved.
//

#ifndef TestOpenGL_JsOpenGLMain_h
#define TestOpenGL_JsOpenGLMain_h

#include "JsOpenGLRasterFont.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>                               // Header File For The GLu32 Library
#include <GLUT/glut.h>                               // Header File For The GLut32 Library
#include <GLUT/glutbitmap.h>  

typedef struct
{
	void *data;
	GLfloat width;
	GLfloat height;
} TextureData;

class JsOpenGLMain
{
	
private :
	JsOpenGLRasterFont *jsOpenGLRasterFont;
//	TextureData td;
	GLuint      texture[1];                         // Storage For One Texture ( NEW )
	GLuint textureGlow;
	int viewWidth;
	int viewHeight;	
	float spriteX;
	float spriteY;
	float spriteXinc;
	float spriteYinc;
	
public:
	
	JsOpenGLMain(int, int);
	~JsOpenGLMain();
	void drawAnObject(void);
	void drawScene(void);
	void RenderGlow(int);
	void RenderToTexture(void);
	GLuint EmptyTexture(void);
	float alpha;	
};




#endif
