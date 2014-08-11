#include "sources.h"
#include "system.h"

namespace SoundLib
{

void MusicSource::onRender()
{
	state[1] = state[0];
	state[0] = *static_cast<MusicSourceState*>(this);
	render();
}

void Music3DSource::onRender()
{
	const Matrix34f& L = system.getRenderListenerPlace();
	float r = L.p.distanceTo(position) + 1e-12f;
	float vol = volume * min(radius / r, 1.0f);
	if(vol < system.mute3DSoundsCoef)
		vol = 0;
	Vector3f d = (position - L.p);
	if(!d.trySelfNormalizeFast())
		d.setZero();
	float pn = Base::atan2(L.x.dot(d), -L.z.dot(d)) * gcfInvPi;

	state[1] = state[0];
	state[0] = *static_cast<MusicSourceState*>(this);
	state[0].volume = vol;
	state[0].pan = pn;

	render();
}

void MusicSource::render()
{
	if(!state[0].active && !state[1].active){
		deactivate();
		return;
	}

	if(!data)
		return;

	if(_setProgressCmd >= 0){
		progress = min((uint)_setProgressCmd, data->frames());
		_setProgressCmd = -1;
	}

	boolean virtualPlaying = state[0].volume == 0.0f && state[1].volume == 0.0f;

	uint P = 0;
	uint D = AUDIOLIB_PACKET_FRAMES;
	float channelVolume[2][AUDIOLIB_MAX_CHANNELS];
	if(state[0].progressJumpPointEnable)
		while((int)progress <= state[0].progressJumpPoint && (int)(progress + D) > state[0].progressJumpPoint)
		{
			uint d = (uint)state[0].progressJumpPoint - progress;
			if(!virtualPlaying)
			{
				for(uint i=0; i < data->channels(); ++i)
				{
					channelVolume[0][i] = volumeInTime(i, P);
					channelVolume[1][i] = volumeInTime(i, P + d);
				}
				system.addMusicWave(data->data(progress, d, true), data->channels(), channelVolume[0], channelVolume[1], P, d);
			}
			progress = state[0].progressJumpPointTarget;
			D -= d;
			P += d;
		}

	minimize(progress, data->frames());
	minimize(D, data->frames() - progress);
	if(!virtualPlaying)
	{
		for(uint i=0; i < data->channels(); ++i)
		{
			channelVolume[0][i] = volumeInTime(i, P);
			channelVolume[1][i] = volumeInTime(i, P + D);
		}
		system.addMusicWave(data->data(progress, D, state[1].active), data->channels(), channelVolume[0], channelVolume[1], P, D);
	}
	progress += D;

	if(state[0].progressJumpPointEnable && progress == data->frames() && state[0].progressJumpPoint == progress)
		progress = state[0].progressJumpPointTarget;

	if(progress == data->frames())
		deactivate();
}

MusicSource::MusicSource(SoundSystem& sys, ISoundData& data) : ISoundSource(sys)
{
	_setProgressCmd = -1;
	progress = 0;
	this->data = &data;
	_selfOwned = false;
	mayDeactivateCount = 0;
}

Music3DSource::Music3DSource(SoundSystem& sys, ISoundData& data) : MusicSource(sys, data)
{
	position.setZero();
	radius = 1.0f;
}

}