#pragma once

#include "array.h"

namespace Base
{
	
template<class T>class IDelegate;
template<class T>class MethodDelegate;
template<class T>class FunctionDelegate;
template<class T>class DummyDelegate;
template<class T>class Delegate;
template<class T>class Event;

static const uint gcDelegateSize = sizeof(void*) * 6;

#define DELEGATE_CODE_GEN_DECLARATION_PLUS 
#define DELEGATE_CODE_GEN_METHOD_TYPES 
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES 
#define DELEGATE_CODE_GEN_METHOD_VARIABLES 

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0, class t_Arg1
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0, t_Arg1
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0, t_Arg1 arg1
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0, arg1

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0, class t_Arg1, class t_Arg2
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0, t_Arg1, t_Arg2
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0, t_Arg1 arg1, t_Arg2 arg2
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0, arg1, arg2

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0, class t_Arg1, class t_Arg2, class t_Arg3
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0, t_Arg1, t_Arg2, t_Arg3
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0, t_Arg1 arg1, t_Arg2 arg2, t_Arg3 arg3
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0, arg1, arg2, arg3

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0, class t_Arg1, class t_Arg2, class t_Arg3, class t_Arg4
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0, t_Arg1, t_Arg2, t_Arg3, t_Arg4
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0, t_Arg1 arg1, t_Arg2 arg2, t_Arg3 arg3, t_Arg4 arg4
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0, arg1, arg2, arg3, arg4

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0, class t_Arg1, class t_Arg2, class t_Arg3, class t_Arg4, class t_Arg5
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0, t_Arg1, t_Arg2, t_Arg3, t_Arg4, t_Arg5
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0, t_Arg1 arg1, t_Arg2 arg2, t_Arg3 arg3, t_Arg4 arg4, t_Arg5 arg5
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0, arg1, arg2, arg3, arg4, arg5

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0, class t_Arg1, class t_Arg2, class t_Arg3, class t_Arg4, class t_Arg5, class t_Arg6
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0, t_Arg1, t_Arg2, t_Arg3, t_Arg4, t_Arg5, t_Arg6
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0, t_Arg1 arg1, t_Arg2 arg2, t_Arg3 arg3, t_Arg4 arg4, t_Arg5 arg5, t_Arg6 arg6
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0, arg1, arg2, arg3, arg4, arg5, arg6

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


#define DELEGATE_CODE_GEN_DECLARATION_PLUS ,class t_Arg0, class t_Arg1, class t_Arg2, class t_Arg3, class t_Arg4, class t_Arg5, class t_Arg6, class t_Arg7
#define DELEGATE_CODE_GEN_METHOD_TYPES t_Arg0, t_Arg1, t_Arg2, t_Arg3, t_Arg4, t_Arg5, t_Arg6, t_Arg7
#define DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES t_Arg0 arg0, t_Arg1 arg1, t_Arg2 arg2, t_Arg3 arg3, t_Arg4 arg4, t_Arg5 arg5, t_Arg6 arg6, t_Arg7 arg7
#define DELEGATE_CODE_GEN_METHOD_VARIABLES arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7

#include "delegate_impl.h"

#undef DELEGATE_CODE_GEN_DECLARATION_PLUS
#undef DELEGATE_CODE_GEN_METHOD_TYPES
#undef DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES
#undef DELEGATE_CODE_GEN_METHOD_VARIABLES


}