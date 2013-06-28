//
//  main.m
//  TestOpenGLnoxib
//
//  Created by Jozef Skrabo on 26/06/12.
//  Copyright (c) 2012 Jozef Skrabo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "JsOpenGLView.h"

#define INT8		int8_t
#define INT16		int16_t
#define INT32		int32_t
#define UINT8		uint8_t
#define UINT16	uint16_t
#define UINT32	uint32_t
#define BYTE		unsigned char



int main(int argc, const char * argv[])
{

	@autoreleasepool
	{	    
		NSLog(@"Hello, World!");
		
		NSLog([[NSBundle mainBundle] bundlePath]);
		
		[NSApplication sharedApplication];
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
		auto menubar = [NSMenu new];
		auto appMenuItem = [NSMenuItem new];
		[menubar addItem:appMenuItem];
		[NSApp setMainMenu:menubar];
		auto appMenu = [NSMenu new];
		auto appName = [[NSProcessInfo processInfo] processName];
		auto quitTitle = [@"Quit " stringByAppendingString:appName];
		auto quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle action:@selector(terminate:) keyEquivalent:@"q"];
		[appMenu addItem:quitMenuItem];
		[appMenuItem setSubmenu:appMenu];
		//id textWiew = [NSTextView alloc];
		
		auto window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 1280-16*4, 800-16*4) styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO];
		auto window_frame = [window frame];
		auto *cv = [[NSView alloc] initWithFrame:window_frame];
		[cv setAutoresizesSubviews:YES];
		[window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
		[window setTitle:appName];
		[window makeKeyAndOrderFront:nil];
		[window setContentView:cv];
		
		JsOpenGLView *openGlView = [[JsOpenGLView alloc] initWithFrame:[[window contentView] frame]];

		//NSOpenGLView *openGlView = [[NSOpenGLView alloc] initWithFrame:[[window contentView] frame]];
		
		auto *scrollview2 = [[NSScrollView alloc] initWithFrame:[[window contentView] frame]];
		auto contentSize2 = [scrollview2 contentSize];
		[scrollview2 setBorderType:NSNoBorder];
		[scrollview2 setHasVerticalScroller:YES];
		[scrollview2 setHasHorizontalScroller:NO];
		[scrollview2 setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		
		auto *theTextView2 = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 0, contentSize2.width, contentSize2.height)];
		[theTextView2 setMinSize:NSMakeSize(0.0, contentSize2.height)];
		[theTextView2 setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
		[theTextView2 setVerticallyResizable:YES];
		[theTextView2 setHorizontallyResizable:NO];
		[theTextView2 setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
		[[theTextView2 textContainer] setContainerSize:NSMakeSize(contentSize2.width, FLT_MAX)];
		//[[theTextView2 textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
		[[theTextView2 textContainer] setWidthTracksTextView:YES];
		[[theTextView2 enclosingScrollView] setHasHorizontalScroller:YES];
		
		auto *myHTMLString = @"<html><a href=\"http://www.dalloz.fr\">my_label</a> <U>Hello</U> <B>World</B> !</html>" ;
		auto *myData = [myHTMLString dataUsingEncoding:NSUTF8StringEncoding];
		auto *textToBeInserted = [[NSAttributedString alloc] initWithHTML:myData documentAttributes:nil];
		[[theTextView2 textStorage] setAttributedString:textToBeInserted];
		for (UINT32 i=0; i<10; i++)
		{
			[[[theTextView2 textStorage] mutableString] appendString: @"<HTML>VLA MON <U>TEXT</U>\n</HTML>"];
		}
		[scrollview2 setDocumentView:theTextView2];
		
		
		
		
		
		//[window setContentView:scrollview];
		auto *splitView = [[NSSplitView alloc] initWithFrame:[[window contentView] bounds]];
//		NSTextView *textView1 = [NSTextView new];
//		NSView *view2 = [NSView new];
		
//		[splitView addSubview:textView1];
//		[splitView addSubview:view2];
		[splitView addSubview:openGlView];
		//[splitView addSubview:scrollview2];
		[splitView adjustSubviews];
		//[splitView setPosition:];
		
		
		[[window contentView] addSubview:splitView];
		//[[window contentView] addSubview:scrollview];
		[window makeKeyAndOrderFront:nil];
		[window makeFirstResponder:theTextView2];
		
		NSLog(@"Start\n");
		fprintf(stdout, "Start\n");
		[NSApp activateIgnoringOtherApps:YES];
		[NSApp run];
		
		
		
		NSLog(@"End\n");
		fprintf(stdout, "End\n");
		return 0;
	}
	return 0;
}














