#import "MyRenderIniter.h"
#import "MyRenderWindowIniter.h"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@implementation MyRenderIniter

- (MyRenderIniter*) init:(NSRunLoop*) loop
{
    glContext = nil;
    displayLink = nil;
    frameNumber = 0;
    renderWindows = [[NSMutableArray alloc] init];
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(onDisplayLink:)];
    [displayLink setFrameInterval:1];
    [displayLink addToRunLoop:loop forMode:NSDefaultRunLoopMode];
    return self;
}

- (void) deinit
{
    [displayLink invalidate];
    //[displayLink release];
}

- (bool) initRender;
{
    glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if(!glContext)
        return false;
    
    if (![EAGLContext setCurrentContext:glContext])
        return false;
    
    glDisable(GL_DITHER);
    
    NSLog(@"GL Context created");
    
    return true;
}

- (void) deinitRender;
{
    [EAGLContext setCurrentContext:nil];
    //[glContext release];
    glContext = nil;
}

- (MyRenderWindowIniter*) createRenderWindowIniter
{
    MyRenderWindowIniter* wi = [[MyRenderWindowIniter alloc] init:self];
    [renderWindows addObject:wi];
    return wi;
}

- (void) deleteRenderWindowIniter:(MyRenderWindowIniter*)wi
{
    [renderWindows removeObject:wi];
    //[wi release];
}

- (void) setSyncInterval:(int)n
{
    [displayLink setFrameInterval:n];
}

- (void) onDisplayLink:(CADisplayLink*)dl
{
    frameNumber ++;
}

- (int) getFrameNumber
{
    return frameNumber;
}

- (EAGLContext*) getGLContext
{
    return glContext;
}


bool MyRenderIniter_initRender(void* slf)
{
    return [(__bridge MyRenderIniter*) slf initRender];
}

void MyRenderIniter_deinitRender(void* slf)
{
    [(__bridge MyRenderIniter*) slf deinitRender];
}

void* MyRenderIniter_createRenderWindowIniter(void* slf)
{
    return (__bridge void *)([(__bridge MyRenderIniter*) slf createRenderWindowIniter]);
}

void MyRenderIniter_deleteRenderWindowIniter(void* slf, void* wi)
{
    MyRenderWindowIniter* p = (__bridge MyRenderWindowIniter*)wi;
    [(__bridge MyRenderIniter*) slf deleteRenderWindowIniter:p];
}

void MyRenderIniter_setSyncInterval(void* slf, int n)
{
    [(__bridge MyRenderIniter*) slf setSyncInterval:n];
}

@end



