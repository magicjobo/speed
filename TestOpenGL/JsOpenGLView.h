//
//  JsOpenGLView.h
//  TestOpenGL
//
//  Created by Jozef Skrabo on 02/06/12.
//  Copyright (c) 2012 Jozef Skrabo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "JsOpenGLMain.h"

static void drawAnObject();

@interface JsOpenGLView : NSOpenGLView
{
}


TextureData loadPngTexture(char *);

//- (void) startAnimationTimer;
- (void)keyDown:(NSEvent *)event;

- (void) drawRect: (NSRect) bounds;
@end
