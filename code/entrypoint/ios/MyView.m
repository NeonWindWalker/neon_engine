#import "MyView.h"
#import "../../entrypoint/ios.h"

@implementation MyView

uint gTouchEventSleepTime = 0;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
	{
		CGPoint location = [touch locationInView:self];
        UITouchPhase phase = [touch phase];
        EntryPoint_onTouch((unsigned int)touch, location.x * self.contentScaleFactor, ((float)self.bounds.size.height - location.y)* self.contentScaleFactor, phase == UITouchPhaseEnded || phase == UITouchPhaseCancelled, phase == UITouchPhaseBegan );

	}
    usleep(gTouchEventSleepTime);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
	{
		CGPoint location = [touch locationInView:self];
        UITouchPhase phase = [touch phase];
        EntryPoint_onTouch((unsigned int)touch, location.x * self.contentScaleFactor, ((float)self.bounds.size.height - location.y)* self.contentScaleFactor, phase == UITouchPhaseEnded || phase == UITouchPhaseCancelled, phase == UITouchPhaseBegan );
	}
    usleep(gTouchEventSleepTime);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
	{
		CGPoint location = [touch locationInView:self];
        EntryPoint_onTouch((unsigned int)touch, location.x * self.contentScaleFactor, ((float)self.bounds.size.height - location.y)* self.contentScaleFactor, true, false );
	}
    usleep(gTouchEventSleepTime);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
	{
		CGPoint location = [touch locationInView:self];
        EntryPoint_onTouch((unsigned int)touch, location.x * self.contentScaleFactor, ((float)self.bounds.size.height - location.y)* self.contentScaleFactor, true, false );
	}
    usleep(gTouchEventSleepTime);
}

@end
