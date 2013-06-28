//
//  JsOpenGLRasterFont.h
//
//  Copyright (c) 2012 Jozef Skrabo. All rights reserved.
//

#ifndef TestOpenGL_JsOpenGLRasterFont_h
#define TestOpenGL_JsOpenGLRasterFont_h

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>                               // Header File For The GLu32 Library
#include <GLUT/glut.h>                               // Header File For The GLut32 Library
#include <GLUT/glutbitmap.h>                               // Header File For The GLut32 Library
#include "JsFontSiesta8x8x1.h"

class JsOpenGLRasterFont
{
	
	private :
	GLuint fontOffset;
	JsFontSiesta8x8x1 *jsFontSiesta8x8x1;
public:
	
	JsOpenGLRasterFont();
	~JsOpenGLRasterFont();
	void makeRasterFont(void);
	void printString(char *);
	void printStringAt(char *, int, int);
	void display(void);
	void myReshape(GLsizei, GLsizei);
	void output(GLfloat, GLfloat, char *);
	void render_string(float, float, float, void *, const char *);
};


#endif
