#ifdef _WIN32
#include "win32FileStorage.cpp"
#endif

#ifdef __linux__ 
#include "stdioFileStorage.cpp"
#include "linuxFileStorage.cpp"
#endif

#ifdef __ANDROID__ 
#include "androidFileStorage.cpp"
#endif

#ifdef __APPLE__
#include "stdioFileStorage.cpp"
#include "iosFileStorage.cpp"
#endif

