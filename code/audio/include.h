#pragma once

#include "../base/include.h"

namespace AudioLib
{
    
USING_BASE_LIB
    
#define AUDIOLIB_PACKET_FRAMES 800
#define AUDIOLIB_FRAME_RATE 48000
#define AUDIOLIB_MAX_CHANNELS 8

enum ESpeekersMode
{
	ESpeekersMode_None = 0,

	ESpeekersMode_1_0_Mono = 100,

	ESpeekersMode_2_0_Stereo = 200,
	ESpeekersMode_2_1_Stereo = 311,

	ESpeekersMode_3_0_Stereo = 300,
	ESpeekersMode_3_1_Stereo = 411,
	ESpeekersMode_3_0_Surround = 302,
	ESpeekersMode_3_1_Surround = 413,

	ESpeekersMode_4_0_Quad = 400,
	ESpeekersMode_4_1_Quad = 511,
	ESpeekersMode_4_0_Surround = 402,
	ESpeekersMode_4_1_Surround = 513,

	ESpeekersMode_5_0 = 500,
	ESpeekersMode_5_1 = 610,
	ESpeekersMode_5_0_Side = 500,
	ESpeekersMode_5_1_Side = 610,

	ESpeekersMode_6_0 = 600,
	ESpeekersMode_6_1 = 711,
	ESpeekersMode_6_0_Side = 602,
	ESpeekersMode_6_1_Side = 713,

	ESpeekersMode_7_0 = 700,
	ESpeekersMode_7_1 = 811,
	ESpeekersMode_7_0_Side = 702,
	ESpeekersMode_7_1_Side = 813,
	ESpeekersMode_7_0_Sorround = 704,
	ESpeekersMode_7_1_Sorround = 815
};

static INLINE uint channelsOfSpeekersMode(ESpeekersMode mode)
{
	return uint(mode) / 100;
}

class IAudioOutput
{
public:
	IAudioOutput(){ _channels = 0; }
	virtual ~IAudioOutput(){}
	static IAudioOutput* create(boolean forceStereo = true);

	int16 samples[AUDIOLIB_MAX_CHANNELS * AUDIOLIB_PACKET_FRAMES];

	virtual void active(boolean01 b)=0;
	virtual boolean01 active()const=0;
    virtual boolean01 isMediaPlayerPlaying()const { return false; }
    
	uint channels()const { return _channels; }
	ESpeekersMode speekersMode()const { return _speekersMode; }

	void setCallback(Delegate<void (IAudioOutput& dev)> cb)
	{
		LockGuard lg(_callbackMutex);
		_callback = cb;
	}
	void setEmptyCallback()
	{
		LockGuard lg(_callbackMutex);
		_callback.setEmpty();
	}

protected:
	uint _channels;
	ESpeekersMode _speekersMode;
	Delegate<void (IAudioOutput& dev)> _callback;
	Mutex _callbackMutex;
};
   
}