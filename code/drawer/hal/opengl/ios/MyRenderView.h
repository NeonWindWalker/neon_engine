#pragma once

@interface MyRenderView : UIView

- (float) getDensity;
- (void) setDensity:(float)d;
- (float) getPixelSize;
- (void) getSize:(int*)w :(int*)h;

float MyRenderView_getDensity(void* slf);
void MyRenderView_setDensity(void* slf, float d);
void MyRenderView_getScreenSize(int* w, int* h, void* slf);
float MyRenderView_getPixelSize(void* slf);

@end