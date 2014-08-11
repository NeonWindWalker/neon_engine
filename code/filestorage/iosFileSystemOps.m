#include "iosFileSystemOps.h"

bool MyIOS_CreateDirectory(const char* dir)
{
    NSString *dirStr = [NSString stringWithUTF8String:dir];
    
    BOOL isDir = YES;
    if ([[NSFileManager defaultManager] fileExistsAtPath:dirStr isDirectory:&isDir])
        return true;
        
    NSError *err = 0;
    [[NSFileManager defaultManager] createDirectoryAtPath:dirStr withIntermediateDirectories:NO attributes:nil error:&err];
    if(!err)
        return true;
    NSLog(@"cant create directory %@ %@", dirStr, err.description);
    return false;
}