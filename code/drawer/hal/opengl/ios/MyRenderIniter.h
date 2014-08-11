#pragma once

#import "MyRenderView.h" 
#import <QuartzCore/QuartzCore.h>

@class MyRenderWindowIniter;
@class EAGLContext;

@interface MyRenderIniter : NSObject
{
    EAGLContext* glContext;
    CADisplayLink* displayLink;
    volatile int frameNumber;
    NSMutableArray* renderWindows;
}

- (MyRenderIniter*) init:(NSRunLoop*)loop;
- (void) deinit;

- (bool) initRender;
- (void) deinitRender;
- (MyRenderWindowIniter*) createRenderWindowIniter;
- (void) deleteRenderWindowIniter:(MyRenderWindowIniter*)wnd;
- (void) setSyncInterval:(int)n;

- (EAGLContext*) getGLContext;
- (int) getFrameNumber;
- (void) onDisplayLink:(CADisplayLink*)dl;

//wrappers
bool MyRenderIniter_initRender(void* slf);
void MyRenderIniter_deinitRender(void* slf);
void* MyRenderIniter_createRenderWindowIniter(void* slf);
void MyRenderIniter_deleteRenderWindowIniter(void* slf, void* wi);
void MyRenderIniter_setSyncInterval(void* slf, int n);


@end