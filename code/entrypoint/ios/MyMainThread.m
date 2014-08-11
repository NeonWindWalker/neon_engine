//
//  MainThread.m
//  test
//
//  Created by admin on 22.09.12.
//  Copyright (c) 2012 devgame.ru. All rights reserved.
//

#import "MyMainThread.h"
#import "MyView.h"
#import "../../drawer/hal/opengl/ios/MyRenderIniter.h"
#import "../../entrypoint/ios.h"


@implementation MyMainThread


- (MyMainThread*) init:(MyView*)v
{
    stopFlag = false;
    pauseFlag = false;
    pauseFlagAccedpted = false;
    tryExitFlag = false;
    initializationFinished = false;
    noNeedSentTerminateApplicationMessage = false;
    view = v;
    
    //[MKStoreManager sharedManager];
    
    gTouchEventSleepTime = [[NSProcessInfo processInfo] processorCount] > 1 ? 10000 : 20000;
    
    renderIniter = [[MyRenderIniter alloc] init:[NSRunLoop currentRunLoop]];
    thread = [[NSThread alloc] initWithTarget:self selector:@selector(threadFunc:) object:nil];
    [thread start];
    return self;
}

- (void) stop
{
    noNeedSentTerminateApplicationMessage = true;
    stopFlag = true;
    pauseFlag = false;
    while([thread isExecuting])
        usleep(1000);
    //[thread release];
    [renderIniter deinit];
    //[renderIniter release];
}

- (void) setPause:(bool)b
{
    if(b)
    {
        pauseFlag = true;
        while(!pauseFlagAccedpted)
            usleep(1000);
    }
    else
    {
        pauseFlagAccedpted = false;
        pauseFlag = false;
    }
    
}

- (void) setTryExitFlag
{
    tryExitFlag = true;
}

- (bool) isInitializationFinished
{
    return initializationFinished;
}

- (void) threadFunc:(id)param
{
    NSString* readPath = [[NSBundle mainBundle] resourcePath];
    char readPathBuffer[1024];
    [readPath getCString:readPathBuffer maxLength:1024 encoding:NSASCIIStringEncoding];
    
    NSArray* homePaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* homePath = [homePaths objectAtIndex:0];
    char homePathBuffer[1024];
    [homePath getCString:homePathBuffer maxLength:1024 encoding:NSASCIIStringEncoding];
    
    EntryPoint_initLog("");
    
    //usleep(1000000);
    
    EntryPoint_init(readPathBuffer, homePathBuffer, (__bridge void *)(renderIniter), (__bridge void *)(view));
    
    initializationFinished = true;
    
    bool stepFinishCalled = false;
    while(!stopFlag)
    {
        if(pauseFlag)
        {
            if(!stepFinishCalled){
                EntryPoint_stepFinish();
                stepFinishCalled = true;
            }
            pauseFlagAccedpted = true;
            usleep(250000);
            continue;
        }
        stepFinishCalled = false;
        
        if(!EntryPoint_step())
            break;
    }
    if(!stepFinishCalled)
		EntryPoint_stepFinish();
    EntryPoint_deinit();
    
    //[renderIniter release];
    
    if(!noNeedSentTerminateApplicationMessage)
        exit(0);
        
    //need send terminate application message
}

void MyMainThread_onLogCallback(const char* str)
{
   NSLog(@"%s", str);
}

void MyObjCWrap_openURL(const char* str)
{
    NSString* s = [NSString stringWithUTF8String:str];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString: s]];
}

void MyObjCWrap_openRateUs()
{
    NSString* appID = @"740270746"; //fixme
    
    static NSString *const iOS7AppStoreURLFormat = @"itms-apps://itunes.apple.com/app/id%@?at=10l6dK";
    static NSString *const iOSAppStoreURLFormat = @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@&at=10l6dK";
    
    NSString * url = [NSString stringWithFormat:([[UIDevice currentDevice].systemVersion floatValue] >= 7.0f)? iOS7AppStoreURLFormat: iOSAppStoreURLFormat, appID];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString: url]];
}

void MyObjCWrap_getLanguage(char* lang)
{
    NSString *currentLanguage = [[NSLocale preferredLanguages] objectAtIndex:0];
    [currentLanguage getCString:lang maxLength:31 encoding:NSASCIIStringEncoding];
}

@end
