#pragma once

#import "../../drawer/hal/opengl/ios/MyRenderView.h"

extern uint gTouchEventSleepTime;

@interface MyView : MyRenderView

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;

@end