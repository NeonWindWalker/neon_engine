#include "log.h"
#include "delegate.h"
#include "sync.h"
#include "string.h"
#include "backtrace.h"
#include <stdio.h>

#ifdef _WIN32
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace Base
{

namespace Private
{
	Event<LogPrinter::Semantic_t> gLogPrinterListeners;
	Mutex gLogMutex;
}

LogPrinter::LogPrinter(CodePoint cp, const char* ch) : channel(ch)
{
	codePoint = cp;
	//backtraceString(trace);
}

LogPrinter::~LogPrinter()
{
	LockGuard lg(Base::Private::gLogMutex);
	Base::Private::gLogPrinterListeners(codePoint, trace, constString(channel), msg);
}


LogPrinter& LogPrinter::operator << (const ConstString& str)
{
	msg << str;
	return *this;
}

LogPrinter& LogPrinter::operator << (const char* str)
{
	msg << constString(str);
	return *this;
}

LogPrinter& LogPrinter::operator << (const int32 v)
{
	msg << print(v);
	return *this;
}

LogPrinter& LogPrinter::operator << (const uint32 v)
{
	msg << print(v);
	return *this;
}

LogPrinter& LogPrinter::operator << (const int64 v)
{
	msg << print(v);
	return *this;
}

LogPrinter& LogPrinter::operator << (const uint64 v)
{
	msg << print(v);
	return *this;
}

LogPrinter& LogPrinter::operator << (const float v)
{
	msg << print(v);
	return *this;
}

LogPrinter& LogPrinter::operator << (const double v)
{
	msg << print(v);
	return *this;
}

void LogPrinter::addListener(const Delegate<Semantic_t>& fun)
{
	LockGuard lg(Base::Private::gLogMutex);
	Base::Private::gLogPrinterListeners += fun;
}

void LogPrinter::removeListener(const Delegate<Semantic_t>& fun)
{
	LockGuard lg(Base::Private::gLogMutex);
	Base::Private::gLogPrinterListeners -= fun;
}

void LogPrinter::genStandardFormatMessage(String& outMsg, CodePoint cp, ConstString trace, ConstString ch, ConstString msg)
{
	outMsg.clear(msg.size() + ch.size() + 1024 );
	String file;
	getShortPath(file, constString(cp.file), 2);
	outMsg << ch << _CS(":\t") << msg << printTabs(maximum<int>(0, 64 - msg.size()) / 4) << _CS("\tfunc: ") << constString(cp.function) << _CS("\tfile: ") << file <<_CS("\ttrace: ");
}

void LogPrinter::genStandardFormatShortMessage(String& outMsg, CodePoint cp, ConstString ch, ConstString msg)
{
	outMsg.clear(msg.size() + ch.size() + 1024 );
	outMsg << ch << _CS(":\t") << msg  << _CS("\tfunc: ") << constString(cp.function);
}

void DefaultLogListener::onLog(CodePoint codePoint, Base::ConstString trace, Base::ConstString ch, Base::ConstString msg)
{
	Base::String str;
	Base::LogPrinter::genStandardFormatShortMessage(str, codePoint, ch, msg);
	str << _CS("\n");
	printf("%s", (const char*)str.c_str());
#ifdef _WIN32
	OutputDebugStringA(str.c_str());
#endif
}

DefaultLogListener::DefaultLogListener()
{
	Base::LogPrinter::addListener(makeDelegate(this, &DefaultLogListener::onLog));
}

DefaultLogListener::~DefaultLogListener()
{
	Base::LogPrinter::removeListener(makeDelegate(this, &DefaultLogListener::onLog));
}

}
