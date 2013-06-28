//
//  JsOpenGLView.m
//  TestOpenGL
//
//  Jozef Skrabo 2012
//  Copyright (c) 2012 Jozef Skrabo. All rights reserved.
//

#import "JsOpenGLView.h"

#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>                               // Header File For The GLu32 Library
#include <GLUT/glut.h>                               // Header File For The GLut32 Library
#include <GLUT/glutbitmap.h>                               // Header File For The GLut32 Library
#include "JsOpenGLRasterFont.h"
#include "JsOpenGLMain.h"

@implementation JsOpenGLView

NSTimer *animationTimer = nil;
static int initOpenGlViewFlag = 0;	

int currentWidth = 512;
int currentHeight = 512;

TextureData td;
JsOpenGLMain *jsOpenGLMain = NULL;

void myinit(void)
{
	glShadeModel (GL_FLAT);
	
	
	jsOpenGLMain = new JsOpenGLMain(currentWidth, currentHeight);

}

- (TextureData)loadPdfTexture:(NSString *)fileName
{
	// NSImage supports the first page of PDFs out of the box. Just create the NSImage as usual with -initWithContentsOfURL:, etc.
	
	//	NSData *pdfData = [[NSData alloc] initWithContentsOfURL:[NSURL URLWithString:@"test.pdf"]];
	//		void *pdfData = (__bridge void *)[[NSData alloc] initWithContentsOfURL:[NSURL URLWithString:@"test.pdf"]];
	
	/*
	 NSData *pdfData = [NSData dataWithContentsOfURL:[NSURL URLWithString:@"http://localhost/test/test.pdf"]];
	 NSPDFImageRep *img = [NSPDFImageRep imageRepWithData:pdfData]; 
	 NSFileManager *fileManager = [NSFileManager defaultManager];
	 int count = [img pageCount]; 
	 for(int i = 0 ; i < count ; i++)
	 { 
	 [img setCurrentPage:1]; 
	 NSImage *temp = [[NSImage alloc] init]; 
	 [temp addRepresentation:img]; 
	 NSBitmapImageRep *rep = [NSBitmapImageRep imageRepWithData:[temp TIFFRepresentation]]; 
	 NSData *finalData = [rep representationUsingType:NSJPEGFileType properties:nil]; 
	 NSString *pageName = [NSString stringWithFormat:@"Page_%d.jpg", [img currentPage]]; 
	 [fileManager createFileAtPath:[NSString stringWithFormat:@"%@/%@", @"/Users/mac/Desktop/", pageName] contents:finalData attributes:nil];
	 }	
	 */
	
	//------
	CFURLRef textureURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("test"), CFSTR("pdf"), NULL);
	NSAssert(textureURL, @"Texture name invalid");
	
	CGImageSourceRef imageSource = CGImageSourceCreateWithURL(textureURL, NULL);
	NSAssert(imageSource, @"Invalid Image Path.");
	NSAssert((CGImageSourceGetCount(imageSource) > 0), @"No Image in Image Source.");
	CFRelease(textureURL);
	
	CGImageRef image = CGImageSourceCreateImageAtIndex(imageSource, 0, NULL);
	NSAssert(image, @"Image not created.");
	CFRelease(imageSource);
	
	GLuint width = CGImageGetWidth(image);
	GLuint height = CGImageGetHeight(image);
	
	void *data = malloc(width * height * 4);
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	NSAssert(colorSpace, @"Colorspace not created.");
	
	CGContextRef context = CGBitmapContextCreate(data, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
	NSAssert(context, @"Context not created.");	
	CGColorSpaceRelease(colorSpace);
	// Flip so that it isn't upside-down
	CGContextTranslateCTM(context, 0, height);
	CGContextScaleCTM(context, 1.0f, -1.0f);
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
	CGImageRelease(image);
	CGContextRelease(context);
	
	
	for (int y=0; y < height; y+=4)
	{
		for (int x=0; x < width; x++)
		{
	 		//*(unsigned char *)((char *)data + x*4 + y*width*4 + 0)= 0xFF;	// B
	 		//*(unsigned char *)((char *)data + x*4 + y*width*4 + 1)= 0xFF;  // G
	 		//*(unsigned char *)((char *)data + x*4 + y*width*4 + 2)= 0xFF;	// R
	 		//*(unsigned char *)((char *)data + x*4 + y*width*4 + 3)= 0xFF;  // A
			((unsigned char *)data)[x*4 + y*width*4 + 0] = 0xFF;
			((unsigned char *)data)[x*4 + y*width*4 + 1] = 0xFF;
			((unsigned char *)data)[x*4 + y*width*4 + 2] = 0xFF;
			((unsigned char *)data)[x*4 + y*width*4 + 3] = 0xFF;
		}
	}
	
	
	return (TextureData){ data, static_cast<GLfloat>(width), static_cast<GLfloat>(height) };
}

TextureData loadPngTexture(char *inputFileName)
{
	//NSString *fileName = [NSString stringWithUTF8String:inputFileName];
//	CFURLRef textureURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(), (__bridge CFStringRef)fileName, CFSTR("png"), NULL);
	//CFURLRef textureURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(),CFSTR("windows_64"), CFSTR("png"), CFSTR("DATA"));
	//CFURLRef textureURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(),CFSTR("windows_64"), CFSTR("png"), NULL);
	
//	NSCAssert(textureURL, [@"Texture name invalid : " stringByAppendingString:fileName]);
	
//	CGImageSourceRef imageSource = CGImageSourceCreateWithURL(textureURL, NULL);
//	NSCAssert(imageSource, @"Invalid Image Path.");
//	NSCAssert((CGImageSourceGetCount(imageSource) > 0), @"No Image in Image Source.");
//	CFRelease(textureURL);
	
	//
	auto dataProvider = CGDataProviderCreateWithFilename(inputFileName);
	auto image = CGImageCreateWithPNGDataProvider(dataProvider, NULL, NO, kCGRenderingIntentDefault);
	
//	CGImageRef image = CGImageSourceCreateImageAtIndex(imageSource, 0, NULL);
//	NSCAssert(image, @"Image not created.");
//	CFRelease(imageSource);
	
	GLuint width = CGImageGetWidth(image);
	GLuint height = CGImageGetHeight(image);
	
	void *data = malloc(width * height * 4);
	
	auto colorSpace = CGColorSpaceCreateDeviceRGB();
	NSCAssert(colorSpace, @"Colorspace not created.");
	
	auto context = CGBitmapContextCreate(data, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
	NSCAssert(context, @"Context not created.");	
	CGColorSpaceRelease(colorSpace);
	// Flip so that it isn't upside-down
	CGContextTranslateCTM(context, 0, height);
	CGContextScaleCTM(context, 1.0f, -1.0f);
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
	CGImageRelease(image);
	CGContextRelease(context);
	
	for (int y=0; y < height; y+=1)
	{
		for (int x=0; x < width; x++)
		{
			/*
			 * (char *)(data + x*4 + y*width*4 + 0)= 0xFF;	// B
			 * (char *)(data + x*4 + y*width*4 + 1)= 0x00;  // G
			 * (char *)(data + x*4 + y*width*4 + 2)= 0x00;	// R
			 * (char *)(data + x*4 + y*width*4 + 3)= 0xFF;  // A
			 */
			//((unsigned char *)data)[x*4 + y*width*4 + 0] = 0xFF;	// B
			//((unsigned char *)data)[x*4 + y*width*4 + 1] = 0xFF;		// G
			//((unsigned char *)data)[x*4 + y*width*4 + 2] = 0xFF;  // R
			
			// set alpha for particlar transparency 
			if (((unsigned char *)data)[x*4 + y*width*4 + 0] == 0x00)
			{
				//((unsigned char *)data)[x*4 + y*width*4 + 3] = 0x00;	// A
			}	
		}
	}
	
	return (TextureData){ data, static_cast<GLfloat>(width), static_cast<GLfloat>(height) };
}


// ==========================================================================================================================
- (void) startAnimationTimer
{
	if (animationTimer == nil)
	{
		//animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
		animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
		//animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.005 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
	}
}

- (void) stopAnimationTimer
{
	if (animationTimer != nil)
	{
		[animationTimer invalidate];
		animationTimer = nil;
	}
}

- (void) animationTimerFired:(NSTimer *)timer
{
	//Scene *scene = [openGLView scene];
	//[scene advanceTimeBy:0.017];
	//[openGLView setNeedsDisplay:YES];
	//drawOpenGlView(1);	
	[self setNeedsDisplay:YES];
}

- (void) initOpenGlView
{
	if (initOpenGlViewFlag == 0)
	{
		
		//NSViewController *controller = [[NSViewController alloc] initWithNibName:@"View"     bundle:nil];
		//NSPopover *popover = [[NSPopover alloc] init];
		//[popover setContentSize:NSMakeSize(100.0f, 100.0f)];
		//[popover setContentViewController:controller];
		//[popover setAnimates:YES];
		//[popover showRelativeToRect:[sender bounds] ofView:sender preferredEdge:NSMaxXEdge];

		
		
		
		//NSString *fullPath = [[NSBundle mainBundle] pathForResource:@"Default" ofType:@"png" inDirectory:@""];
		
		NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
		NSString *filePath = @"/DATA/windows_64.png";
		NSString *fullFilePath = [bundlePath stringByAppendingString:filePath];
		const char *charFullFilePath = [fullFilePath UTF8String];			
		
		NSAlert *alert = [[NSAlert alloc] init];
		[alert  addButtonWithTitle:@"Amiga is the best computer ever..."];
		[alert addButtonWithTitle:@"Amiga is the best computer ever..."];
		[alert addButtonWithTitle:@"Amiga is the best computer ever..."];
		//[alert setMessageText:@"Delete the record?"];
		[alert setMessageText:@"Software Failure.    Press left mouse button to continue.\nGuru Meditation #00000004.0000AAC0"];
		//[alert setInformativeText:@"Deleted records cannot be restored."];
		[alert setInformativeText:fullFilePath];
		[alert setAlertStyle:NSWarningAlertStyle];
		
		if ([alert runModal] == NSAlertFirstButtonReturn)
		{
			// OK clicked, delete the record
		}
		
		td = loadPngTexture((char *) charFullFilePath);
		// GL_TEXTURE_2D pour des tailles de textures en puissances de 2
		// GL_TEXTURE_RECTANGLE_ARB pour des tailles de textures arbitraires
		glBindTexture(GL_TEXTURE_2D, 0);	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, td.width, td.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, td.data);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		
		filePath = @"/DATA/crystal_sphere.png";
		fullFilePath = [bundlePath stringByAppendingString:filePath];
		charFullFilePath = [fullFilePath UTF8String];		
		td = loadPngTexture((char *) charFullFilePath);
		// GL_TEXTURE_2D pour des tailles de textures en puissances de 2
		// GL_TEXTURE_RECTANGLE_ARB pour des tailles de textures arbitraires
		glBindTexture(GL_TEXTURE_2D, 1);	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, td.width, td.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, td.data);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		filePath = @"/DATA/sphere.png";
		//filePath = @"/DATA/bunker_hdri.png";
		
		fullFilePath = [bundlePath stringByAppendingString:filePath];
		charFullFilePath = [fullFilePath UTF8String];		
		td = loadPngTexture((char *) charFullFilePath);
		// GL_TEXTURE_2D pour des tailles de textures en puissances de 2
		// GL_TEXTURE_RECTANGLE_ARB pour des tailles de textures arbitraires
		glBindTexture(GL_TEXTURE_2D, 2);	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, td.width, td.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, td.data);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		
		
		myinit();
		initOpenGlViewFlag = 1;
	}
}

- (id)initWithFrame:(NSRect)frame
{
	
	NSSize size = frame.size;
	NSLog(@"size: %@", NSStringFromSize(size));
	NSLog(@"size: %f x %f", size.width, size.height);
	currentWidth = size.width;
	currentHeight = size.height;	
	
	NSOpenGLPixelFormat * pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:(NSOpenGLPixelFormatAttribute[]) {
		NSOpenGLPFAWindow,
		//NSOpenGLPFAFullScreen, 
		NSOpenGLPFAAlphaSize, 8, 
		NSOpenGLPFAColorSize, 24, 
		NSOpenGLPFAPixelBuffer, 
		//NSOpenGLPFAAcceleratedCompute, 
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated, 
		NSOpenGLPFADepthSize, 16,
		nil
	}];
/*	
kCGLPFAColorSize, colorBits, 
kCGLPFADepthSize, depthBits,
	kCGLPFAFullScreen,
	kCGLPFAAccelerated, kCGLPFADoubleBuffer }	
*/	
	
	if(pixelFormat == NULL)
		NSLog(@"pixelFormat == NULL");
	

		self = [super initWithFrame:frame pixelFormat:pixelFormat];
		if(self == NULL) {
			NSLog(@"Could not initialise self");
			return NULL;
		}
		
		[[self openGLContext] makeCurrentContext];
		//[self initGL];
	[self startAnimationTimer];
	[self initOpenGlView];
	
	return self;
}

/*
- (id)initWithFrame:(NSRect)frame
{
	
	return self;
}
*/




-(void) drawRect: (NSRect) bounds
{

	jsOpenGLMain->drawScene();
	[[NSOpenGLContext currentContext] flushBuffer];
}


- (void)reshape
{
	//NSLog(@"reshaping...");
	[[self openGLContext]makeCurrentContext];
	auto bounds = [self bounds];
	//NSLog(@"reshaping: %f, %f", NSWidth(bounds), NSHeight(bounds));
	glViewport(0, 0, NSWidth(bounds), NSHeight(bounds));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -2, 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	[NSOpenGLContext clearCurrentContext];
}

- (void)keyDown:(NSEvent *)event
{
	//NSLog(@"keyDown");
	//printf("KeyDown");
	//alpha = alpha + 0.2f;
	//glRotatef(alpha, 1.0, 1.0, 1.0);
	//drawAnObject();
	//glFlush();
	[self setNeedsDisplay:YES];
}

// enable key input
- (BOOL)acceptsFirstResponder
{
	return YES;
}



@end
