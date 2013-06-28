//
//  JsOpenGLRasterFont.cpp
//
//  Copyright (c) 2012 Jozef Skrabo. All rights reserved.
//

#include "JsOpenGLRasterFont.h"

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>                               // Header File For The GLu32 Library
#include <GLUT/glut.h>                               // Header File For The GLut32 Library
#include <GLUT/glutbitmap.h>                               // Header File For The GLut32 Library
#include "JsFontSiesta8x8x1.h"


//=============================================================================================================
// constructor
JsOpenGLRasterFont::JsOpenGLRasterFont()
{
	jsFontSiesta8x8x1 = new JsFontSiesta8x8x1();
	makeRasterFont();
}

//=============================================================================================================
// destructor
JsOpenGLRasterFont::~JsOpenGLRasterFont()
{
	
}

//=============================================================================================================
void JsOpenGLRasterFont::makeRasterFont(void)
{
	GLuint i;
	
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	unsigned char *siesta_plan = jsFontSiesta8x8x1->getSiestaPlan();
	fontOffset = glGenLists (128);
	for (i = 0; i < 255; i++) {
		glNewList(i+fontOffset, GL_COMPILE);
		glBitmap(8, 8, 0.0, 0.0, 8.0, 0.0, (GLubyte *) (siesta_plan + i*8));
		glEndList();
	}
}

//=============================================================================================================
void JsOpenGLRasterFont::printString(char *s)
{
	glPushAttrib (GL_LIST_BIT);
	glListBase(fontOffset);
	glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
	glPopAttrib ();
}

//=============================================================================================================
void JsOpenGLRasterFont::printStringAt(char *s, int x, int y)
{
	glRasterPos2i(x,  y+8);
	printString(s);
}

//=============================================================================================================
void JsOpenGLRasterFont::display(void)
{
	GLfloat white[3] = { 1.0, 1.0, 1.0 };
	int i, j;
	char teststring[33];
	
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3fv(white);
	
	
	for (i = 0; i < 127; i += 32)
	{
		glRasterPos2i(20, 200 - 8*i/32);
		for (j = 0; j < 32; j++)
			teststring[j] = (char) (i+j);
		teststring[32] = 0;
		printString(teststring);
	}
}

//=============================================================================================================
void JsOpenGLRasterFont::myReshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0.0, w, 0.0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

//=============================================================================================================
void JsOpenGLRasterFont::output(GLfloat x, GLfloat y, char *text)
{
	char *p;
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	for (p = text; *p; p++)
		glutStrokeCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	glPopMatrix();
}

//=============================================================================================================
void JsOpenGLRasterFont::render_string(float x, float y, float z, void* font, const char* s)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2f(x, y);
	
	while(*s)
	{
		glutBitmapCharacter(font, *s);
		s++;
	}
}

