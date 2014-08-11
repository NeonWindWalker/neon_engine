#include "include.h"

#ifdef __ANDROID__

#include "../filestorage/myjni.h"

namespace AudioLib
{
    
class AndroidAudioOutput : public IAudioOutput
{
public:
    boolean01 _play;
    jobject javaImplimentation;
	jmethodID jplayMethodId;
    
    AndroidAudioOutput()
    {
		_play = false;
		_channels = 2;
		_speekersMode = ESpeekersMode_2_0_Stereo;
		javaImplimentation = 0;
		jplayMethodId = 0;
		
		JNIEnv* env = getThreadJavaEnviroment(gMainJavaVirtualMachine);
		jclass cls = env->FindClass("ru.devgame.AudioOutput");
		if(!cls){
			logError << "find ru.devgame.AudioOutput java class";
			return;
		}

		jmethodID jcreateMethodId = env->GetStaticMethodID(cls, "create", "(I)Lru/devgame/AudioOutput;");
		if(!jcreateMethodId){
			logError << "find create function in ru.devgame.AudioOutput java class";
			return;
		}
			
		jplayMethodId = env->GetMethodID(cls, "play", "(Z)V");
		if(!jplayMethodId){
			logError << "find play method in ru.devgame.AudioOutput java class";
			return;
		}

		javaImplimentation = env->CallStaticObjectMethod(cls, jcreateMethodId, (jint)this);
		if(javaImplimentation == 0){
			logError << "create ru.devgame.AudioOutput";
			return;
		}
		
		javaImplimentation = env->NewGlobalRef(javaImplimentation);
		_play = false;
    }
    
    ~AndroidAudioOutput()
    {
		if(!javaImplimentation)
			return;
		JNIEnv* env = getThreadJavaEnviroment(gMainJavaVirtualMachine);
		jclass cls = env->FindClass("ru.devgame.AudioOutput");
		if(!cls){
			logError << "find ru.devgame.AudioOutput java class";
			return;
		}
		jmethodID jdecreateMethodId = env->GetMethodID(cls, "decreate", "()V");
		if(!jdecreateMethodId){
			logError << "cant find decreate function in ru.devgame.AudioOutput java class";
			return;
		}
		env->CallVoidMethod(javaImplimentation, jdecreateMethodId);
		env->DeleteGlobalRef(javaImplimentation);
    }
    
    virtual void active(boolean01 b)
    {
        if(_play == b)
            return;
		JNIEnv* env = getThreadJavaEnviroment(gMainJavaVirtualMachine);
		env->CallVoidMethod(javaImplimentation, jplayMethodId, (bool)b);
        _play = b;
    }
    
    virtual boolean01 active()const
    {
        return _play;
    }
    
    void doWork()
	{
		if(_play)
		{
			_callbackMutex.lock();
			if(_callback)
				_callback(*this);
			_callbackMutex.unlock();
		}
		else
			Base::memoryClear(samples, AUDIOLIB_PACKET_FRAMES * 2 * sizeof(int16));
	}
};

extern "C" JNIEXPORT void JNICALL JNICALL Java_ru_devgame_AudioOutput_callback(JNIEnv* env, jobject obj, jint cppClassPtr, jshortArray arr)
{
	AndroidAudioOutput* p = reinterpret_cast<AndroidAudioOutput*>(cppClassPtr);
	p->doWork(); env->SetShortArrayRegion(arr, 0							, AUDIOLIB_PACKET_FRAMES * 2, p->samples);
	/*p->doWork(); env->SetShortArrayRegion(arr, AUDIOLIB_PACKET_FRAMES * 2 * 1, AUDIOLIB_PACKET_FRAMES * 2, p->samples);
	p->doWork(); env->SetShortArrayRegion(arr, AUDIOLIB_PACKET_FRAMES * 2 * 2, AUDIOLIB_PACKET_FRAMES * 2, p->samples);
	p->doWork(); env->SetShortArrayRegion(arr, AUDIOLIB_PACKET_FRAMES * 2 * 3, AUDIOLIB_PACKET_FRAMES * 2, p->samples);
	p->doWork(); env->SetShortArrayRegion(arr, AUDIOLIB_PACKET_FRAMES * 2 * 4, AUDIOLIB_PACKET_FRAMES * 2, p->samples);
	p->doWork(); env->SetShortArrayRegion(arr, AUDIOLIB_PACKET_FRAMES * 2 * 5, AUDIOLIB_PACKET_FRAMES * 2, p->samples);
	p->doWork(); env->SetShortArrayRegion(arr, AUDIOLIB_PACKET_FRAMES * 2 * 6, AUDIOLIB_PACKET_FRAMES * 2, p->samples);
	p->doWork(); env->SetShortArrayRegion(arr, AUDIOLIB_PACKET_FRAMES * 2 * 7, AUDIOLIB_PACKET_FRAMES * 2, p->samples);*/
}


IAudioOutput* IAudioOutput::create(boolean bFoceStereo)
{
    return new AndroidAudioOutput();
}
    
}

#endif

