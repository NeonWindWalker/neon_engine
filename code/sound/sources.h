#include "core.h"

namespace SoundLib
{

class MusicSourceState
{
protected:
	boolean active;
public:
	float volume;
	float pan;
	int progressJumpPoint;
	uint progressJumpPointTarget;
	boolean progressJumpPointEnable;
	MusicSourceState()
	{
		active = false;
		volume = 1.0f;
		pan = 0.0f;
		progressJumpPoint = -1;
		progressJumpPointTarget = 0;
		progressJumpPointEnable = false;
	}
friend class MusicSource;
};

class MusicSource : public ISoundSource, public MusicSourceState
{
public:
	MusicSource(SoundSystem& sys, ISoundData& data);

	void onRender();

	void setLooped()
	{
		progressJumpPoint = data->frames();
		progressJumpPointTarget = 0;
		progressJumpPointEnable = true;
	}

	void start()
	{
		_setProgressCmd = 0;
		if(active)
			return;
		activate();
	}

	void stop()
	{
		active = false;
		_setProgressCmd = 0;
	}

	void play()
	{
		if(data && progress == data->frames())
			_setProgressCmd = 0;
		if(active)
			return;
		
		activate();
	}

	void pause()
	{
		active = false;
	}

	void play(boolean b)
	{
		if(b)
			play();
		else
			pause();
	}

	ISoundData& getData()
	{
		return *data;
	}

	void selfOwn()
	{ 
		_selfOwned = true;
		spinLock.lock();
		if(_selfOwn.get() != this)
			_selfOwn = this;
		spinLock.unlock();
	}

	void selfUnOwn()
	{ 
		_selfOwned = false;
		spinLock.lock();
		_selfOwn = 0;
		spinLock.unlock();
	}

	boolean isSelfOwned()const
	{
		return _selfOwned;
	}

protected:
	MusicSourceState state[2];
	volatile int _setProgressCmd;
	uint progress;
	void render();

	float volumeInTime(uint ch, uint frame)
	{
		float tay = float(frame) * (1.0f / float(AUDIOLIB_PACKET_FRAMES));
		float pan = state[1].pan + (state[0].pan - state[1].pan) * tay;
		float panCoef;
		if(ch & 1)
			panCoef = min(1.0f - pan, 1.0f);
		else
			panCoef = min(1.0f + pan, 1.0f);
		float v0 = state[0].active ? state[0].volume : 0.0f;
		float v1 = state[1].active ? state[1].volume : 0.0f;
		return (v1 + (v0 - v1) * tay) * panCoef;
	}

	StrongPtrThreadSafe<ISoundData> data;
	StrongPtrThreadSafe<MusicSource> _selfOwn;
	boolean _selfOwned;
	SpinLock spinLock;

	INLINE void activate()
	{
		spinLock.lock();
		active = true;
		if(_selfOwned && _selfOwn.get() != this)
			_selfOwn = this;
		renderCallbackActive(true);
		spinLock.unlock();
	}

	uint mayDeactivateCount;
	INLINE void deactivate()
	{
		mayDeactivateCount ++;
		if(mayDeactivateCount < 8)
			return;

		StrongPtrThreadSafe<MusicSource> cp;
		spinLock.lock();
		renderCallbackActive(false);
		active = false;
		mayDeactivateCount = 0;
		cp = _selfOwn;
		_selfOwn = 0;
		spinLock.unlock();

		cp = 0;
	}
};


class Music3DSource : public MusicSource
{
public:
	Music3DSource(SoundSystem& sys, ISoundData& data);

	Vector3f position;
	float radius;

	void onRender();
};

template <class t_Source>
class TVolumeFadedSoundSource : public t_Source
{
public:
	float fadeSmoothTime;

	TVolumeFadedSoundSource(SoundSystem& sys, ISoundData& data) : t_Source(sys, data)
	{
		fadeSmoothTime = 1.0f;
	}

	virtual void onRender()
	{
		if(t_Source::_selfOwn.getRefCount() <= 1)
			t_Source::volume = smoothVolumeValue(t_Source::volume, fadeSmoothTime, float(AUDIOLIB_PACKET_FRAMES) / float(AUDIOLIB_FRAME_RATE), 0.0f);
		t_Source::onRender();
	}
};


/*
class SoundMusicBeepSource : public SoundMusicSource
{
public:
	SoundMusicBeepSource(SoundSystem& sys, ISoundData& data, float volume) : SoundMusicSource(sys, data)
	{
		selfOwn = this;
		this->volume = volume;
		active(true);
	}

	void onRender()
	{
		if(data)
		{
			SoundMusicSource::onRender();
			if(progress == data->frames())
				selfOwn = 0;
		}
		else
			selfOwn = 0;
	}

private:
	StrongPtrThreadSafe<SoundMusicBeepSource> selfOwn;
};

*/


}