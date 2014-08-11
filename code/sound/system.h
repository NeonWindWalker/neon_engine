#pragma once

#include "core.h"

#define SOUNDLIB_INT16_TO_INT32_SCALE_DEG 12
#define SOUNDLIB_INT16_TO_INT32_SCALE (1<<SOUNDLIB_INT16_TO_INT32_SCALE_DEG)
#define SOUNDLIB_INT16_TO_INT32_SCALE_FLT float(SOUNDLIB_INT16_TO_INT32_SCALE)

namespace SoundLib
{

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	void attach(IAudioOutput* dev);
	IAudioOutput* device()const { return _device; }

	void registerSource(ISoundSource* src);
	void unregisterSource(ISoundSource* src);

	Matrix34f listenerPlace;

	struct SpeekerInfo
	{
		Vector3f position;

		Vector3f onListenerSpherePosition;
		Vector3f direction;
		float toListenerSphereDistance;

		SpeekerInfo()
		{
			position.set(0.0f, 0.0f, 0.0f);
			onListenerSpherePosition.set(0.0f, 0.0f, 0.0f);
			direction.set(0.0f, 0.0f, 0.0f);
			toListenerSphereDistance = 0.0f;
		}

		void calculate(float listenerRadius)
		{
			if(position.lenghtSquare() > 1e-6f)
			{
				direction = - position.normal();
				onListenerSpherePosition = - direction * listenerRadius;
			}
			else
			{
				onListenerSpherePosition.setZero();
				direction.setZero();
			}
			toListenerSphereDistance = position.lenght() - listenerRadius;
		}
	};

	float mute3DSoundsCoef;
	SpeekerInfo speekersInfo[AUDIOLIB_MAX_CHANNELS];
	ESpeekersMode speekersMode()const { return _speekersMode; }
	boolean speekersMode(ESpeekersMode mode);

	float listenerRadius()const { return _listenerRadius; }
	void listenerRadius(float val);

	void addMusicWave(const int16* src, uint channels, const float* volume0, const float* volume1, uint pos = 0, uint frames = AUDIOLIB_PACKET_FRAMES);
	
	const Matrix34f& getRenderListenerPlace()const
	{
		return _listenerPlace;
	}

	const Matrix34f& getRenderOldListenerPlace()const
	{
		return _listenerPlaceOld;
	}

	void setCallback(Delegate<void (SoundSystem& dev)> cb)
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
	DECLARE_ALIGN(16) int32 musicSamples[AUDIOLIB_PACKET_FRAMES][2];
	DECLARE_ALIGN(16) float samples[AUDIOLIB_PACKET_FRAMES * AUDIOLIB_MAX_CHANNELS];
	boolean samplesUsed;

	Matrix34f _listenerPlace;
	Matrix34f _listenerPlaceOld;

	float _listenerRadius;
	IAudioOutput* _device;
	ESpeekersMode _speekersMode;
	uint _channels;

	Mutex elementsListLock;
	ListRoot<ISoundElement> elementsList;
	Array<ISoundElement*> releasedElementsList;

	Mutex renderedSourcesLock;
	ListRoot<ISoundElement> renderedSources;
	Array<ISoundElement*> renderedSourcesReplyc;

	Delegate<void (SoundSystem& dev)> _callback;
	Mutex _callbackMutex;

	friend class ISoundElement;
	friend class ISoundSource;

	void calculateSpeekersInfo();
	float speekerCoef(float dot) { return dot * 0.5f + 0.5f; }
	void onGenAudioOutput(IAudioOutput& dev);

	void garbadgeWork();

	INLINE int16 fastConvertAndClampToInt16(int32 i)
	{
		int32 converted = int32(uint32(i + int32(1<<30))>>SOUNDLIB_INT16_TO_INT32_SCALE_DEG) - int32(1<<(30 - SOUNDLIB_INT16_TO_INT32_SCALE_DEG));
		return (int16)clamp(converted, -32767, 32767);
	}
};


}