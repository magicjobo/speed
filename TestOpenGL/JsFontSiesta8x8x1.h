//
//  FontSiesta8x8x1.h
//  TestOpenGL
//
//  Created by Jozef Skrabo on 23/06/12.
//  Copyright (c) 2012 Jozef Skrabo. All rights reserved.
//

#ifndef TestOpenGL_FontSiesta8x8x1_h
#define TestOpenGL_FontSiesta8x8x1_h



class JsFontSiesta8x8x1
{
	
private :
	unsigned char * siestaPlan;
	//unsigned char siestaPlan[128][8];
	
public:
	
	JsFontSiesta8x8x1();
	~JsFontSiesta8x8x1();
	unsigned char *getSiestaPlan(void);
};



#endif
