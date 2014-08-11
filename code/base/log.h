#pragma once

#include "seed.h"
#include "string.h"
#include "vector.h"
#include "delegate.h"
#include "threading.h"

#ifdef FINAL_RELEASE
	#define BASE_CONFIG_COMMENT_LOG_FATAL
	#define BASE_CONFIG_COMMENT_LOG_ERROR
	#define BASE_CONFIG_COMMENT_LOG_WARNING
	#define BASE_CONFIG_COMMENT_LOG_INFO
	#define BASE_CONFIG_COMMENT_LOG_DEBUG
#endif

#ifndef DEBUG
	#define BASE_CONFIG_COMMENT_LOG_DEBUG
#endif


#ifndef BASE_CONFIG_COMMENT_LOG_FATAL
#define logFatal Base::LogPrinter(Base::CodePoint(__FILE__, __LINE__, __FUNCTION__), "fatal")
#else
#define logFatal Base::LogPrinterDummy()
#endif

#ifndef BASE_CONFIG_COMMENT_LOG_ERROR
#define logError Base::LogPrinter(Base::CodePoint(__FILE__, __LINE__, __FUNCTION__), "error")
#else
#define logError Base::LogPrinterDummy()
#endif

#ifndef BASE_CONFIG_COMMENT_LOG_WARNING
#define logWarning Base::LogPrinter(Base::CodePoint(__FILE__, __LINE__, __FUNCTION__), "warning")
#else
#define logWarning Base::LogPrinterDummy()
#endif

#ifndef BASE_CONFIG_COMMENT_LOG_INFO
#define logInfo Base::LogPrinter(Base::CodePoint(__FILE__, __LINE__, __FUNCTION__), "info")
#define logInfoSkim Base::LogPrinter(Base::CodePoint(__FILE__, __LINE__, __FUNCTION__), "info")
#else
#define logInfo Base::LogPrinterDummy()
#define logInfoSkim  Base::LogPrinterDummy()
#endif

#ifndef BASE_CONFIG_COMMENT_LOG_DEBUG
#define logDebug Base::LogPrinter(Base::CodePoint(__FILE__, __LINE__, __FUNCTION__), "debug")
#else
#define logDebug Base::LogPrinterDummy()
#endif

namespace Base
{

struct LogPrinter
{
	CodePoint codePoint;
	String trace;
	const char* channel;
	String msg;

	LogPrinter(CodePoint cp, const char* ch);
	~LogPrinter();
	LogPrinter& operator << (const ConstString& str);
	LogPrinter& operator << (const char* str);
	LogPrinter& operator << (const int32 n);
	LogPrinter& operator << (const uint32 n);
	LogPrinter& operator << (const int64 n);
	LogPrinter& operator << (const uint64 n);
	LogPrinter& operator << (const float f);
	LogPrinter& operator << (const double n);

	typedef void Semantic_t(CodePoint codePoint, ConstString trace, ConstString channel, ConstString msg);

	static void addListener(const Delegate<Semantic_t>& fun);
	static void removeListener(const Delegate<Semantic_t>& fun);

	static void genStandardFormatMessage(String& outMsg, CodePoint codePoint, ConstString trace, ConstString channel, ConstString msg);
	static void genStandardFormatShortMessage(String& outMsg, CodePoint codePoint, ConstString channel, ConstString msg);
};

struct LogPrinterDummy
{
	LogPrinterDummy(){}

	template<class T>
	LogPrinterDummy& operator << (const T& t){ return *this; }
};

class DefaultLogListener
{
public:
	void onLog(CodePoint codePoint, Base::ConstString trace, Base::ConstString ch, Base::ConstString msg);
	DefaultLogListener();
	~DefaultLogListener();
};

static INLINE void logCompileInfo()
{
logInfoSkim<<_CS("Compiler:");
#ifdef _MSC_VER
	logInfoSkim<<_CS("	MSC");
#endif

#ifdef __GNUG__
	logInfoSkim<<_CS("	gcc");
#endif

logInfoSkim<<_CS("Architecture:");
#ifdef ARCH_ARM
	logInfoSkim << _CS("	ARM");
#endif
/*
#ifdef ARCH_ARMv5
	logInfoSkim<<_CS("ARCH_ARMv5");
#endif

#ifdef ARCH_ARMv7
	logInfoSkim<<_CS("ARCH_ARMv7");
#endif
*/
#ifdef ARCH_X86
	logInfoSkim<<_CS("	X86");
#endif

#ifdef ARCH_X64
	logInfoSkim<<_CS("	X64");
#endif


#ifdef ARCH_IA64
	logInfoSkim<<_CS("	IA64");
#endif

logInfoSkim<<_CS("Platform:");
#ifdef _WIN32
	logInfoSkim<<_CS("	win32");
#endif

#ifdef _WIN64
	logInfoSkim<<_CS("	win64");
#endif

#ifdef __linux__
	logInfoSkim<<_CS("	linux");
#endif

#ifdef __ANDROID__
	logInfoSkim<<_CS("	android");
#endif

#ifdef DEBUG
	logInfoSkim<<_CS("DEBUG");
#endif

#ifdef BIG_ENDIAN_ARCH
	logInfoSkim<<_CS("BIG_ENDIAN_ARCH");
#endif

#ifdef LITTLE_ENDIAN_ARCH
	logInfoSkim<<_CS("LITTLE_ENDIAN_ARCH");
#endif

#ifdef INSTRUCTIONS_THUMB
	logInfoSkim<<_CS("INSTRUCTIONS_THUMB");
#endif

#ifdef INSTRUCTIONS_THUMB2
	logInfoSkim<<_CS("INSTRUCTIONS_THUMB2");
#endif

#ifdef INSTRUCTIONS_SOFTWARE_FPU
	logInfoSkim<<_CS("INSTRUCTIONS_SOFTWARE_FPU");
#endif

#ifdef INSTRUCTIONS_VFP
	logInfoSkim<<_CS("INSTRUCTIONS_VFP");
#endif

#ifdef INSTRUCTIONS_SIMD
	logInfoSkim<<_CS("INSTRUCTIONS_SIMD");
#endif

#ifdef INSTRUCTIONS_SSE
	logInfoSkim<<_CS("INSTRUCTIONS_SSE");
#endif

#ifdef INSTRUCTIONS_SSE2
	logInfoSkim<<_CS("INSTRUCTIONS_SSE2");
#endif

#ifdef INSTRUCTIONS_SSE3
	logInfoSkim<<_CS("INSTRUCTIONS_SSE3");
#endif

#ifdef INSTRUCTIONS_SSE4
	logInfoSkim<<_CS("INSTRUCTIONS_SSE4");
#endif

#ifdef INSTRUCTIONS_AVX
	logInfoSkim<<_CS("INSTRUCTIONS_AVX");
#endif

#ifdef INSTRUCTIONS_NEON
	logInfoSkim<<_CS("INSTRUCTIONS_NEON");
#endif

#ifdef NO_EXCEPTIONS
	logInfoSkim<<_CS("NO_EXCEPTIONS");
#endif

#ifdef NO_WCHAR_SUPPORT
	logInfoSkim<<_CS("NO_WCHAR_SUPPORT");
#endif

logInfoSkim << _CS("INT_SIZE = ") << uint(INT_SIZE);

}

}
