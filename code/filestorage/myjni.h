#pragma once

#include <jni.h>

#include "../base/string.h"
#include "../base/log.h"

extern JavaVM* gMainJavaVirtualMachine;

static INLINE Base::String stringFromJString(JNIEnv * env, jstring jstr)
{
	const char *str;
	str = env->GetStringUTFChars(jstr, 0);
	Base::String s = Base::constString(str);
	env->ReleaseStringUTFChars(jstr, str);
	return s;
}

static INLINE  jstring jStringFromString(JNIEnv* env, Base::ConstString cstr)
{
	Base::String str = cstr;
	return env->NewStringUTF(str.c_str());
}

static INLINE JNIEnv* getThreadJavaEnviroment(JavaVM* jVM)
{
	JNIEnv* env = 0;
	jVM->GetEnv((void**)&env, JNI_VERSION_1_6);
	if(env == 0)
		jVM->AttachCurrentThread(&env, 0);
	if(env == 0)
		logFatal << "find thread java env";
	return env;
}