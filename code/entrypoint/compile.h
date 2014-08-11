#ifdef _WIN32
#include "windows.cpp"
#include "windowsInput.cpp"
#include "windowsGetScreenPhysicalSize.cpp"
#endif

#if defined(__linux__) && !defined(__ANDROID__)
#include "linux.cpp"
#include "linuxInput.cpp"
#endif

#ifdef __ANDROID__
#include "android.cpp"
#endif

#ifdef __APPLE__
#include "ios.cpp"
#endif

#include "purchase.cpp"