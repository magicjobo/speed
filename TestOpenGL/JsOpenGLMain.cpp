//============================================================================
// Name        : JsOpenGLMain.cpp
// Author      : Jozef Skrabo
// Version     : 0.0.1
// Copyright   : Copyright (c) 2012 Jozef Skrabo. All rights reserved.
// Description : Main OpenGL animation loop in C++
//============================================================================
// [ ] alpha channel
// [ ] sprites
// [ ] plain faces
//
// [ ] texturing
//   [ ] load multiple textures
//
// [ ] display consuming time and frame rate
// [ ] display bitmap text with 8x8 font
//   [ ] using 8x8 SIESTA font, C64 font and finally adding support for PCF X11 font format to include Terminus font with unicode encoding
//   [ ] color managment
//   [ ] character screen buffer, virtually bigger than screen
//   [ ] integrate ncuses interfacing
// [ ] simple object loader (commencer le dev d'un loader OPEN COLLADA)
// [ ] simple SVG loader
// [x] lighting (normal)
// [ ] using normal maps
// [ ] using fragment shaders
// [ ] using vertex shaders
// [x] using lists
// [ ] using physics library like bullet 3d
// [x] loading PNG texture
// [ ] loading PDF texture
// [ ] Bones amination(multi obj)
// [ ] Screen Text Buffer (+ color and special attributes)
//============================================================================






// using pixel buffer objects
// using stencil buffer
// using floating point textures
// using half-precision floating point coords

// using sphere env mapping
// using cube env mapping
// using bump mapping
// implementing pseudo HDR
// implementing pseudo motion blur
// implementing pseudo flied of depth blur
// implementing quad anti-alias
// implementig pseudo glow effect
// implementig pseuso SVG Rendering
// using vector fonts (from SVG ?)
// implementing pseudo ambiant occlusion shading with occlusion map precalc and specific shader

// realtime compilation of GLSL shaders

// simple scene ray-tracing and fractals with OpenCL

// bouncing spere dot
// integrate video capture and realtime processing with OpenCV
// soul calibur motion effects

// space invader on (a)steroids


//============================================================================

#include "JsOpenGLMain.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <GLUT/glutbitmap.h>  
#include <math.h>    	// pour les sin/cos
//#include <graphviz/graph.h>

using namespace std; 	// for std::cout


GLuint teapotIndex;

//std::vector<std::string> v = { "xyzzy", "plugh", "abracadabra" };
//std::vector<std::string> v{ "xyzzy", "plugh", "abracadabra" };
//std::
JsOpenGLMain::JsOpenGLMain(int widthParameter, int heightParameter)
{
	viewWidth = widthParameter;
	viewHeight = heightParameter;
	//TextureData td;
	//GLuint      texture[1];                         // Storage For One Texture ( NEW )
	textureGlow = 1;
	alpha = 10.0;	
	spriteX = 200;
	spriteY = 50;
	spriteXinc = 5;
	spriteYinc = 0.01;
	jsOpenGLRasterFont = new JsOpenGLRasterFont();

	teapotIndex = glGenLists(1);
	glNewList(teapotIndex, GL_COMPILE);
		glutSolidTeapot(1.3);
	glEndList();
	
	
	auto lambdafunc = [] () { cout << "Hello world"; };	// c++11 test
	lambdafunc();	// c++11 test


}

JsOpenGLMain::~JsOpenGLMain()
{
	glDeleteLists(teapotIndex, 1);
}

/*
 
 // compile the display list, store a triangle in it
 ...
 
 // draw the display list
 */

// =====================================
GLuint JsOpenGLMain::EmptyTexture( void )
{
	GLuint txtnumber;
	//	unsigned int* data;
	
	// On alloue de la place pour notre texture de 512x512
	//data = (unsigned int*) new GLuint[((512 * 512)* 4 * sizeof(unsigned int))];
	// On nettoie les données en les remplissant de 0
	//ZeroMemory(data,((512 * 512)* 4 * sizeof(unsigned int)));
	
	glGenTextures(1, &txtnumber); // On génère un identifiant
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txtnumber);
	// On effectue la première passe
	//glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	//delete [] data;
	
	glDisable(GL_TEXTURE_2D);
	
	return txtnumber;
}
// =====================================
void JsOpenGLMain::RenderToTexture( void )
{
	/*
	//RenderCube(  );                            //On dessine notre cube
	drawAnObject();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureGlow); //On selectionne notre texture de glow
	// Et on copie le contenu de l'écran vers la texture
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);
	
	glClearColor(0., 0., 0., 0.);              // On efface l'écran avec du noir
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	glDisable(GL_TEXTURE_2D);                  //On deselectionne notre texture
  */
}

//==================================================
void JsOpenGLMain::RenderGlow( int times )
{
	
	//On se projette en vision orthogonale (2D)
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, viewWidth , 0, viewHeight, -1, 1 );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);	
	glBindTexture(GL_TEXTURE_2D,textureGlow);            // On selectionne notre texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // activation de la transparence
	
	glBegin(GL_QUADS);
	for (int i=0; i<times; i++) {
		glColor4f(1., 1., 1., 0.3/(float)i); //Transparence croissante (amusez vous avec)
		
		glTexCoord2f(0+((float)i/75.f),0+((float)i/75.f)); // on "écarte" notre 
		glVertex2f(0,0);                                   // texture progressivement
		
		glTexCoord2f(0+((float)i/75.f),1-((float)i/75.f));
		glVertex2f(0,512);
		
		glTexCoord2f(1-((float)i/75.f),1-((float)i/75.f));
		glVertex2f(512,512);
		
		glTexCoord2f(1-((float)i/75.f),0+((float)i/75.f));
		glVertex2f(512,0);
	}
	glEnd();
	
	// On repasse en vision en perspective 
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
}






// ==========================================================
void JsOpenGLMain::drawAnObject(void)
{
	
	/*	
	 glBegin(GL_POLYGON);
	 glVertex2f(-0.5, -0.5);
	 glVertex2f(-0.5, 0.5);
	 glVertex2f(0.5, 0.5);
	 glVertex2f(0.5, -0.5);
	 glEnd();
	 */
	
	
	/*
	 // We tell we want to draw quads
	 glBegin(GL_QUADS);
	 
	 // Every four calls to glVertex, a quad is drawn
	 
	 glColor3f(0.0, 0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	 glColor3f(0.0, 0.0, 1.0); glVertex3f(-1.0, -1.0,  1.0);
	 glColor3f(0.0, 1.0, 1.0); glVertex3f(-1.0,  1.0,  1.0);
	 glColor3f(0.0, 1.0, 0.0); glVertex3f(-1.0,  1.0, -1.0);
	 
	 glColor3f(1.0, 0.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);
	 glColor3f(1.0, 0.0, 1.0); glVertex3f( 1.0, -1.0,  1.0);
	 glColor3f(1.0, 1.0, 1.0); glVertex3f( 1.0,  1.0,  1.0);
	 glColor3f(1.0, 1.0, 0.0); glVertex3f( 1.0,  1.0, -1.0);
	 
	 glColor3f(0.0, 0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	 glColor3f(0.0, 0.0, 1.0); glVertex3f(-1.0, -1.0,  1.0);
	 glColor3f(1.0, 0.0, 1.0); glVertex3f( 1.0, -1.0,  1.0);
	 glColor3f(1.0, 0.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);
	 
	 glColor3f(0.0, 1.0, 0.0); glVertex3f(-1.0,  1.0, -1.0);
	 glColor3f(0.0, 1.0, 1.0); glVertex3f(-1.0,  1.0,  1.0);
	 glColor3f(1.0, 1.0, 1.0); glVertex3f( 1.0,  1.0,  1.0);
	 glColor3f(1.0, 1.0, 0.0); glVertex3f( 1.0,  1.0, -1.0);
	 
	 glColor3f(0.0, 0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	 glColor3f(0.0, 1.0, 0.0); glVertex3f(-1.0,  1.0, -1.0);
	 glColor3f(1.0, 1.0, 0.0); glVertex3f( 1.0,  1.0, -1.0);
	 glColor3f(1.0, 0.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);
	 
	 glColor3f(0.0, 0.0, 1.0); glVertex3f(-1.0, -1.0,  1.0);
	 glColor3f(0.0, 1.0, 1.0); glVertex3f(-1.0,  1.0,  1.0);
	 glColor3f(1.0, 1.0, 1.0); glVertex3f( 1.0,  1.0,  1.0);
	 glColor3f(1.0, 0.0, 1.0); glVertex3f( 1.0, -1.0,  1.0);
	 
	 glEnd();
	 */
	if (1)
	{

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		//		glEnable(GL_DEPTH_TEST);			
		
		
		glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
		//glEnable(GL_CULL_FACE);   	
		glEnable(GL_BLEND);                         // Enable Blending       (disable alpha testing)
		//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);	// couleur
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// modulation (lumiere)
		
		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
		//glEnable(GL_TEXTURE_GEN_S); 
		//glEnable(GL_TEXTURE_GEN_T);		
    //glEnable(GL_TEXTURE_1D);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_AUTO_NORMAL);
    //glEnable(GL_NORMALIZE);
    //glFrontFace(GL_CW);
    //glCullFace(GL_BACK);		
		
		glBindTexture(GL_TEXTURE_2D, 2);
		
		glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexCoord2f(256, 256);

		
		
		glutSolidTorus(1.0, 4.0, 100, 100);
		//glTranslatef(2.0, 0.0, 0.0);
		//glutSolidSphere(0.5, 50, 50);
		//glTranslatef(-3.0, 0.0, 0.0);

		
		glBindTexture(GL_TEXTURE_2D, 1);

/*		
		glActiveTextureARB(GL_TEXTURE0);	
		glEnable(GL_TEXTURE_2D);
		//Bind diffuse with alpha here
		glBindTexture(GL_TEXTURE_2D, 1);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_MODULATE);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB,GL_PREVIOUS);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_RGB,GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB,GL_TEXTURE );
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND1_RGB,GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_MODULATE);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,GL_PREVIOUS);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA,GL_SRC_ALPHA);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA,GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND1_ALPHA,GL_SRC_ALPHA);
		
		glActiveTextureARB(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		//Bind sphere map here
		glBindTexture(GL_TEXTURE_2D, 0);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_INTERPOLATE);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB,GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_RGB,GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB,GL_PREVIOUS);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND1_RGB,GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB,GL_PREVIOUS);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND2_RGB,GL_SRC_ALPHA);
		glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_REPLACE);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,GL_PRIMARY_COLOR);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA,GL_SRC_ALPHA);
*/		
		glRotatef(sin(alpha*1.0)*180.0 + cos(alpha*11.0)*180.0, 1.0f, 0.0f, 0.0f);
		glRotatef(sin(alpha*2.0)*180.0 + cos(alpha*7.0)*180.0 , 0.0f, 100.0f, 0.0f);
		glRotatef(sin(alpha*3.0)*180.0 + cos(alpha*5.0)*180.0 , 0.0f, 0.0f, 100.0f);

		
		glBegin(GL_QUADS);
		// Front Face
		
		glNormal3f(0.0, 0.0, 1.0);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
		// Back Face
		glNormal3f(0.0, 0.0, -1.0);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
		// Top Face
		glNormal3f(0.0, 1.0, 0.0);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		// Bottom Face
		glNormal3f(0.0, -1.0, 0.0);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		// Right face
		glNormal3f(1.0, 0.0, 0.0);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		// Left Face
		glNormal3f(-1.0, 0.0, 0.0);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
		
		 
		 glEnd();
		
		glDisable(GL_CULL_FACE);   	
/*		
		glBegin(GL_TRIANGLES);
		{
			glTexCoord2f(0.0f, 0.0f);	glRasterPos2i(10, 10);
			glTexCoord2f(1.0f, 0.0f);	glRasterPos2i(0, 10);
			glTexCoord2f(1.0f, 1.0f);	glRasterPos2i(10, 0);		
		}
		glEnd();
*/				
		glDisable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
	}
	glPopAttrib();

	
	//glColor3f(1.0f, 0.85f, 0.35f);
	glClearDepth(1.0f);                         // Depth Buffer Setup
	//glDepthFunc(GL_LEQUAL);                         // Type Of Depth Testing
	//glEnable(GL_DEPTH_TEST);                        // Enable Depth Testing
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);          // Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);                         // Enable Blending       (disable alpha testing)
	
	//  glAlphaFunc(GL_GREATER,0.1f);                       // Set Alpha Testing     (disable blending)
	//glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping
	//glEnable(GL_CULL_FACE);   	
	
	glDisable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);

	glDisable(GL_ALPHA_TEST);                        // Enable Alpha Testing  (disable blending)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // activation de la transparence
/*
	glBegin(GL_TRIANGLES);
	{
 		glColor4f(1.0, 0.0, 0.0, 0.0);	glVertex3f( 0.0, 0.5, 2.0);
 		glColor4f(0.0, 1.0, 0.0, 0.0);	glVertex3f(-0.5,-0.5, 2.0);
 		glColor4f(0.0, 0.0, 1.0, 1.0);	glVertex3f( 0.5,-0.5, 2.0);		
	}
	glEnd();
*/	
	glEnable(GL_ALPHA_TEST);                        // Enable Alpha Testing  (disable blending)
	glDisable(GL_BLEND);
	
	
	
	
	
	
	
	// ==[ VARIABLES ]=========================================
	
	// l0_pos contient la position de la source 0.
	// l0_amb contient la couleur ambiente de la source 0.
	// l0_dif contient la couleur diffuse de la source 0.
	// l0_spe contient la couleur speculaire de la source 0.
	// l0_shi contient l'exposant de phong de la source 0.
	
	//	float l0_pos[4] = {0.0, 0.0, 2.0, 0.0};
	//	float l0_amb[4] = {1.0, 1.0, 1.0, 1.0};
	//	float l0_dif[4] = {1.0, 1.0, 1.0, 1.0};
	//	float l0_spe[4] = {1.0, 1.0, 1.0, 1.0};
	//	float l0_shi = 50.0;
	
	// abc_amb contient la composante ambiente de la matiere
	// abc_dif contient la composante diffuse de la matiere
	// mat_specular contient la composante spéculaire de la matiere
	
	//	float abc_amb[4] = {0.1, 0.1, 0.1, 1.0};
	//	float abc_dif[4] = {1.0, 1.0, 1.0, 1.0};
	//	float mat_specular[4] = {1.0, 1.0, 1.0, 1.0};
	
	// pA, pB, et pC sont 3 tableaux de 2 valeurs contenant
	// les positions des sommets A, B, et C.
	
	float pA[2] = {0.0, 0.0};
	float pB[2] = {2.0, 2.0};
	float pC[2] = {2.0, 0.0};
	
	// tA, tB, et tC sont 3 tableaux de 2 valeurs contenant
	// les coordonnées des points de l'espace de texture
	// correspondant au sommets A, B, et C.
	
	float tA[2] = {0.0, 0.0};
	float tB[2] = {1.0, 1.0};
	float tC[2] = {1.0, 0.0};
	
	// N est un tableau de 3 valeurs représentant les
	// 3 compostantes de la normale à ABC.
	
	float N[3] = {0.0, 0.0, 1.0};
	
	// texID est l'identifiant de la texture appliquée à ABC.
	
	//	unsigned int texID = 0;
	
	// ==[ CODE ]==============================================
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);	
	
	glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
	
	//glutSolidSphere (1.0, 20, 16);
	//-----
	//glutSolidTeapot(1.3);
//	glCallList(teapotIndex);

	//glutSolidCube(1.5);
	glShadeModel(GL_FLAT);                        // Enable Smooth Shading
	
	//glTexEnv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// modulation (lumiere)
	
	glEnable(GL_TEXTURE_2D);
	/*
	 glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);
	 glLightfv(GL_LIGHT0, GL_AMBIENT, l0_amb);
	 glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_dif);
	 glLightfv(GL_LIGHT0, GL_SPECULAR, l0_spe);
	 glLightf(GL_LIGHT0, GL_SHININESS, l0_shi);
	 
	 glEnable(GL_LIGHT0);
	 glEnable(GL_TEXTURE_2D);
	 
	 glBindTexture(GL_TEXTURE_2D, texID);
	 */	
	glBegin(GL_TRIANGLES);
	
	//		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, abc_amb);
	//		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, abc_dif);
	//		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
/*	
	glNormal3f(N[0], N[1], N[2]);
	
	glTexCoord2f(tA[0], tA[1]);
	glVertex2f(pA[0], pA[1]);
	
	glTexCoord2f(tB[0], tB[1]);
	glVertex2f(pB[0], pB[1]);
	
	glTexCoord2f(tC[0], tC[1]);
	glVertex2f(pC[0], pC[1]);
*/	
	glEnd();
	glDisable(GL_LIGHTING);
	
	// Draw blue text at screen coordinates (100, 120), where (0, 0) is the top-left of the
	// screen in an 18-point Helvetica font
	//glRasterPos2i(0, 0);
	//glBitmap(0, 0, 0, 0, 0, 0, 0);
	//glutBitmapString(GLUT_BITMAP_HELVETICA_18, "text to render");
	//glDisable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth(2.0);
	//output(200, 225, "This is antialiased.");	
	
	//glutSwapBuffers();
	//---------------------------------------
	
	//for (int i = 0; i < 32; i++) {glutSetColor (16+i, 1.0*(i/32.0), 1.0*(i/32.0), 0.0);}
 	//glColorTableEXT(GL_COLOR_TABLE, GL_RGBA, 256, GL_RGBA, GL_UNSIGNED_BYTE, rgbaTable);
	//glEnable(GL_COLOR_TABLE);
	
	
	// GL_UNPACK_SWAP_BYTES,   GL_UNPACK_LSB_FIRST,   GL_UNPACK_ROW_LENGTH, GL_UNPACK_IMAGE_HEIGHT, GL_UNPACK_SKIP_PIXELS, GL_UNPACK_SKIP_ROWS, GL_UNPACK_SKIP_IMAGES, GL_UNPACK_ALIGNMENT.
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 8);
	
  //glRasterPos2i(16, 256);
  //glBitmap(8, 256, 0, 0, 0, 8, siesta);
	
  //glRasterPos2i(0, 16);
	//glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	//glPixelStoref(GL_, 1);
  //glBitmap(128, 512,0, 0, 0, 0, siesta);
	//render_string(10.0, 20.0, 0.0, GLUT_BITMAP_8_BY_13, "SYNTAX ERROR !"); 
	
}




void JsOpenGLMain::drawScene(void)
{
	
	glDisable(GL_DITHER);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_FOG);
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glPixelZoom(1.0,1.0);
	
	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();                           // Reset The Projection Matrix
	
	//int width = 512;
	//int height = 512;
	
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)viewWidth/(GLfloat)viewHeight,0.1f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
	glLoadIdentity();	
	
	glBindTexture(GL_TEXTURE_2D, 1);

	
	//glClearColor(0, 0, 0, 0);
	
	glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black Background
	glClearDepth(1.0);                         // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 	
	
	/* Clear the buffer, clear the matrix */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);	
	glLoadIdentity();
	
	GLfloat light_position[] = { 0.0, 0.0, 10.0, 0.0 };
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	//glShadeModel(GL_SMOOTH);
	glShadeModel(GL_FLAT);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	/* A step backward, then spin the cube */
	glTranslatef(0.0, 0.0, -10.0);
	//glTranslatef(-1.5f,0.0f,-6.0f);
	//glRotatef(30, 1, 0, 0);
	glRotatef(sin(alpha*3.0)*180.0 + cos(alpha*7.0)*180.0, 1.0f, 0.0f, 0.0f);
	glRotatef(sin(alpha*11.0)*180.0 + cos(alpha*5.0)*180.0 , 0.0f, 100.0f, 0.0f);
	glRotatef(sin(alpha*13.0)*180.0 + cos(alpha*3.0)*180.0 , 0.0f, 0.0f, 100.0f);
	
	
	drawAnObject();
	//RenderToTexture();
	//RenderGlow(1);
	
	
		glBindTexture(GL_TEXTURE_2D, 1);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, viewWidth, viewHeight, 0.0, -viewWidth, viewWidth);	// set to "standard" raster coordinates
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	glBegin(GL_QUADS);
	int spritePosX = spriteX;
	int spritePosY = spriteY;
	glTexCoord2f(0.0f,1.0f); glVertex3i(  spritePosX, spritePosY+256, 0);
	glTexCoord2f(0.0f,0.0f); glVertex3i(  spritePosX, spritePosY, 0);
	glTexCoord2f(1.0f,0.0f); glVertex3i(spritePosX+256, spritePosY, 0);
	glTexCoord2f(1.0f,1.0f); glVertex3i(spritePosX+256, spritePosY+256, 0);
	spriteX += spriteXinc;
	spriteY += spriteYinc;
	if ((spriteX < 0.0) || (spriteX > (viewWidth - 256.0)))
	{
		spriteXinc = -spriteXinc;
		spriteX += spriteXinc*2.0;
	}
	if ((spriteY < 0.0) || (spriteY > (viewHeight - 228.0)))
	{
		spriteYinc = -spriteYinc;
		//spriteY += spriteYinc*2.0;
		spriteY += spriteYinc;
	}

	if (spriteY > 0.0) 
	{
		spriteYinc += 2;
	}
	
	glEnd();
	
	
	glDisable(GL_TEXTURE_2D);	
	
	glShadeModel(GL_FLAT);
	glIndexi(1);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	//render_string(8, 512 - 32, 0, GLUT_BITMAP_TIMES_ROMAN_10, textToDisplay); 
	jsOpenGLRasterFont->printStringAt( (char *) "Test OPEN GL", 8, 8);
	jsOpenGLRasterFont->printStringAt( (char *) "============", 8, 16);
	jsOpenGLRasterFont->printStringAt( (char *) "============", 8, 24);

	
	char textToDisplay [128];
  int textLength;
  textLength = sprintf (textToDisplay, "ALPHA : %f", alpha);
	jsOpenGLRasterFont->printStringAt(textToDisplay, 8, 32);
	
	glColor4f(1.0, 1.0, 0.0, 0.0);
	int line = 6;
	sprintf (textToDisplay, "spriteX : %03f", spriteX);jsOpenGLRasterFont->printStringAt(textToDisplay, 8, (line++)*8);
	sprintf (textToDisplay, "spriteY : %06f", spriteY);jsOpenGLRasterFont->printStringAt(textToDisplay, 8, (line++)*8);
	glColor4f(1.0, 0.0, 1.0, 1.0);
	sprintf (textToDisplay, "spriteXinc : %06f", spriteXinc);jsOpenGLRasterFont->printStringAt(textToDisplay, 8, (line++)*8);
	sprintf (textToDisplay, "spriteYinc : %06f", spriteYinc);jsOpenGLRasterFont->printStringAt(textToDisplay, 8, (line++)*8);
	jsOpenGLRasterFont->display();

//	glFlush();
	
	//NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:[NSFont fontWithName:@"Helvetica" size:26], NSFontAttributeName,[NSColor yellowColor], NSForegroundColorAttributeName, nil];	
	//NSAttributedString * currentText=[[NSAttributedString alloc] initWithString:@"Cat" attributes: attributes];
	//[currentText drawAtPoint:NSMakePoint(10, 10)];	
	
	// Rotate a bit more
	alpha = alpha + 0.001f;
	
}

