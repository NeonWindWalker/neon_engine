#include "backtrace.h"

#ifndef FINAL_RELEASE

#if defined(__linux__) && !defined(__ANDROID__)
#include <execinfo.h>
#include <cxxabi.h>

namespace Base
{

void backtraceAddreses(Array<void*>& outArr)
{
	const uint MAX_FRAMES = 100;
	void* addresses[MAX_FRAMES];
	uint size = backtrace(addresses, MAX_FRAMES);
	outArr.clear(size);
	outArr.insert(0, addresses, size);
}

void backtraceString(String& outStr)
{
	const uint MAX_FRAMES = 100;
	void* addresses[MAX_FRAMES];
	uint size = backtrace(addresses, MAX_FRAMES);
	char** symbols = backtrace_symbols(addresses, size);
	outStr.clear();
	
	for(uint i = 1; i < size; i++) {
            char *begin = 0, *end = 0;
            // find the parentheses and address offset surrounding the mangled name
            for (char *j = symbols[i]; *j; ++j) {
                if (*j == '(') {
                    begin = j;
                } else if (*j == '+') {
                    end = j;
                }
            }
            if (begin && end) {
                *begin++ = ' ';
                *end = 0;
                // found our mangled name, now in [begin, end)

                int status;
                size_t sz;
                char *ret = abi::__cxa_demangle(begin, 0, &sz, &status);
                if (ret) {
                    // return value may be a realloc() of the input
                    outStr << constString(ret) << _CS("\n");
                    free(ret);
                } else {
                    // demangling failed, just pretend it's a C function with no args
                    outStr << constString(symbols[i]) << _CS("\n");
                }

            } else {
                outStr << constString(symbols[i]);
            }
        }
	
	free(symbols);
}

}
#endif

#if defined(__ANDROID__) || defined(__APPLE__)
namespace Base
{

void backtraceAddreses(Array<void*>& outArr)
{
}

void backtraceString(String& outStr)
{
}

}
#endif





#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

#include "alghoritms.h"
namespace Base
{
	void backtraceAddreses(Array<void*>& outArr)
	{
		void* stack[ 256 ];
		uint frames = CaptureStackBackTrace( 0, 256, stack, NULL );
		outArr.clear(frames - 1);
		outArr.push(stack + 1, frames - 1);
	}

	void backtraceString(String& outStr)
	{
		Array<void*> addreses;
		backtraceAddreses(addreses);

		HANDLE process = GetCurrentProcess();
		SymInitialize( process, NULL, TRUE );

		SYMBOL_INFO* symbol = (SYMBOL_INFO*)alloca(sizeof(SYMBOL_INFO) + 1024 * sizeof(char));
		symbol->MaxNameLen   = 1024;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		for(uint i=1; i < addreses.size(); ++i)
		{
			SymFromAddr( process, ( DWORD64 )( addreses[ i ] ), 0, symbol );
			outStr << constString(symbol->Name) << _CS("\n");
		}
	}
}

#endif

#else
namespace Base
{

	void backtraceAddreses(Array<void*>& outArr)
	{
	}

	void backtraceString(String& outStr)
	{
	}
}
#endif
