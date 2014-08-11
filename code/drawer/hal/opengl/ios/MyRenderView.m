#import "MyRenderView.h"
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

@implementation MyRenderView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}


- (id)initWithCoder:(NSCoder*)coder
{
    if ((self = [super initWithCoder:coder]))
    {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        // In this application, we want to retain the EAGLDrawable contents after a call to presentRenderbuffer.
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        //self.contentScaleFactor = 1.0;
        self.contentScaleFactor = [[UIScreen mainScreen] scale];
    }
    
    return self;
}



- (float)getDensity
{
    return self.contentScaleFactor / [[UIScreen mainScreen] scale];
}

- (void) setDensity:(float)d
{
    self.contentScaleFactor = [[UIScreen mainScreen] scale] * d;
}

- (float) getPixelSize
{
    float scale = 1;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
        scale = [[UIScreen mainScreen] scale];
    }
    float dpi;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        dpi = 132 * scale;
    } else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        dpi = 163 * scale;
    } else {
        dpi = 160 * scale;
    }
    
    float ret = 0.0254f / dpi / [self getDensity];
    return ret;
}

- (void) getSize:(int*)w :(int*)h
{
    *w = self.bounds.size.width;
    *h = self.bounds.size.height;
}


float MyRenderView_getDensity(void* slf)
{
    return [(__bridge id) slf getDensity];
}

void MyRenderView_setDensity(void* slf, float d)
{
    return [(__bridge id) slf setDensity:d];
}

void MyRenderView_getScreenSize(int* w, int* h, void* slf)
{
    UIScreen *MainScreen = [UIScreen mainScreen];
    UIScreenMode *ScreenMode = [MainScreen currentMode];
    CGSize Size = [ScreenMode size]; // <--- Real screen size
    *w = Size.width;
    *h = Size.height;
}

float MyRenderView_getPixelSize(void* slf)
{
    return [(__bridge id) slf getPixelSize];
}

@end
