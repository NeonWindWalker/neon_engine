#pragma once

#import "MyRenderView.h"

@class MyRenderIniter;

@interface MyRenderWindowIniter : NSObject
{
    MyRenderIniter* renderIniter;
    GLuint glFramebuffer;
    GLuint glRenderbuffer;
    GLuint glDepthBuffer;
    GLint width;
    GLint height;
    MyRenderView* view;
    int frameNumber;
    int xxxNumber;
}

- (MyRenderWindowIniter*) init:(MyRenderIniter*)parent;

- (int) initRenderWindow:(MyRenderView*)view;
- (void) deinitRenderWindow;
- (int) resize;
- (bool) present;

- (int) getWidth;
- (int) getHeight;


int MyRenderWindowIniter_initRenderWindow(void* slf, void* myRenderView);
void MyRenderWindowIniter_deinitRenderWindow(void* slf);
int MyRenderWindowIniter_resize(void* slf);
bool MyRenderWindowIniter_present(void* slf);
void MyRenderWindowIniter_size(void* slf, int* width, int* height);

@end
