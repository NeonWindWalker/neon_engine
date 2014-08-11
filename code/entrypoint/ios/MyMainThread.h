#pragma once

#import "MyView.h"

@class MyRenderIniter;

@interface MyMainThread : NSObject
{
    NSThread* thread;
    volatile bool stopFlag;
    volatile bool pauseFlag;
    volatile bool pauseFlagAccedpted;
    volatile bool tryExitFlag;
    volatile bool initializationFinished;
    volatile bool noNeedSentTerminateApplicationMessage;
    volatile int mayPresentFrame;
    MyView* view;
    MyRenderIniter* renderIniter;
    NSString* appId;
}

- (MyMainThread*) init:(MyView*)v;
- (void) setPause:(bool)b;
- (void) setTryExitFlag;
- (void) stop;
- (bool) isInitializationFinished;

- (void) threadFunc:(id)param;

@end

void MyMainThread_onLogCallback(const char* str);
void MyObjCWrap_openURL(const char* str);
void MyObjCWrap_openRateUs();
void MyObjCWrap_getLanguage(char* lang);