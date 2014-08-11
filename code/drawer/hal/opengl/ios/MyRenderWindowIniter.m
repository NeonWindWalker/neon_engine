#import "MyRenderWindowIniter.h"
#import "MyRenderIniter.h"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@implementation MyRenderWindowIniter

- (MyRenderWindowIniter*) init:(MyRenderIniter*)parent
{
    renderIniter = parent;
    frameNumber = 0;
    return self;
}


- (int) initRenderWindow:(MyRenderView*)v
{
    [self deinitRenderWindow];
    
    view = v;
    glGenFramebuffers(1, &glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    
    glGenRenderbuffers(1, &glRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, glRenderbuffer);
    // This call associates the storage for the current render buffer with the EAGLDrawable (our CAEAGLLayer)
    // allowing us to draw into a buffer that will later be rendered to screen wherever the layer is (which corresponds with our view).
    [[renderIniter getGLContext] renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)view.layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, glRenderbuffer);
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    // For this sample, we also need a depth buffer, so we'll create and attach one via another renderbuffer.
    glGenRenderbuffers(1, &glDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, glDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glDepthBuffer);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return 0;
    }
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    //clear
    /*
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindRenderbuffer(GL_RENDERBUFFER, glRenderbuffer);
    [[renderIniter getGLContext] presentRenderbuffer:GL_RENDERBUFFER];
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    */
    
    return glFramebuffer;
}


- (void) deinitRenderWindow
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &glFramebuffer);
    glFramebuffer = 0;
    glDeleteRenderbuffers(1, &glRenderbuffer);
    glRenderbuffer = 0;
    glDeleteRenderbuffers(1, &glDepthBuffer);
    glDepthBuffer = 0;
    view = nil;
    width = height = 0;
}


- (int) resize
{
    if(!view)
        return 0;
    if(abs((float)width - view.bounds.size.width * view.contentScaleFactor) < 0.5f && abs((float)height - view.bounds.size.height * view.contentScaleFactor) < 0.5f)
        return glFramebuffer;
    MyRenderView* v = view;
    [self deinitRenderWindow];
    return [self initRenderWindow:v];
}

- (bool) present
{
    //glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    /*
    
    if(xxxNumber & 1)
        glClearColor(1.0, 0.0, 1.0, 1.0);
    else
        glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    xxxNumber ++;
    */
     
    
    glFinish();
    // Display the buffer
    glBindRenderbuffer(GL_RENDERBUFFER, glRenderbuffer);
    
    int frame = [renderIniter getFrameNumber];
    while(frame < frameNumber)
    {
        usleep(1000);
        frame = [renderIniter getFrameNumber];
    }
    frameNumber = frame;
    
    
    [[renderIniter getGLContext] presentRenderbuffer:GL_RENDERBUFFER];
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

- (int) getWidth;
{
    return width;
}

- (int) getHeight
{
    return height;
}



int MyRenderWindowIniter_initRenderWindow(void* slf, void* view)
{
    return [(__bridge MyRenderWindowIniter*) slf initRenderWindow:(__bridge MyRenderView*)view];
}

void MyRenderWindowIniter_deinitRenderWindow(void* slf)
{
    return [(__bridge MyRenderWindowIniter*) slf deinitRenderWindow];
}

int MyRenderWindowIniter_resize(void* slf)
{
    return [(__bridge MyRenderWindowIniter*) slf resize];
}

bool MyRenderWindowIniter_present(void* slf)
{
    return [(__bridge MyRenderWindowIniter*) slf present];
}

void MyRenderWindowIniter_size(void* slf, int* width, int* height)
{
    *width = [(__bridge MyRenderWindowIniter*) slf getWidth];
    *height = [(__bridge MyRenderWindowIniter*) slf getHeight];
}


@end
