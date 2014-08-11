#include "system.h"
#include "../../../external/opus/include/opus.h"

namespace SoundLib
{
    
SoundSystem::SoundSystem()
{
	mute3DSoundsCoef = 0.1f;
    _listenerRadius  = 0.1f;
    _speekersMode = ESpeekersMode_1_0_Mono;
	_device = 0;
	_channels = 1;

	listenerPlace.setIdentity();
	_listenerPlace.setIdentity();
	_listenerPlaceOld.setIdentity();
}

/*
CELTMode* SoundSystem::celtMode()
{
	if(_celtMode)
		return _celtMode;

	int err;
	_celtMode = celt_mode_create(AUDIOLIB_FRAME_RATE, 960, &err);
}

CELTEncoder* SoundSystem::celtEndcoder(uint channels)
{
	if(channels > AUDIOLIB_MAX_CHANNELS)
		return 0;
	if(_celtEndcoder[channels])
		return _celtEndcoder[channels];
	int err;
	_celtEndcoder[channels] = celt_encoder_create_custom(_celtMode, channels, &err);
	return _celtEndcoder[channels];
}

CELTDecoder* SoundSystem::celtDecoder(uint channels)
{
	if(channels > AUDIOLIB_MAX_CHANNELS)
		return 0;
	if(_celtDecoder[channels])
		return _celtDecoder[channels];
	int err;
	_celtDecoder[channels] = celt_decoder_create_custom(_celtMode, channels, &err);
	return _celtDecoder[channels];
}
*/

SoundSystem::~SoundSystem()
{
	if(_device)
		_device->setEmptyCallback();

	/*
	elementsListLock.lock();

	foreach(e, releasedElementsList)
		delete *e;
	releasedElementsList.clearSaveReserve();
	
	foreach(e, renderedSources)
		releasedElementsList.push(&*e);

	logError << "Undeleted elements count " << releasedElementsList.size() << " deleting ...";

	foreach(e, releasedElementsList)
		delete *e;
	releasedElementsList.clearSaveReserve();

	elementsListLock.unlock();
	*/


	garbadgeWork();
}

void SoundSystem::attach(IAudioOutput* dev)
{
	_device = dev;
	_device->setCallback(makeDelegate(this, &SoundSystem::onGenAudioOutput));
	_speekersMode = _device->speekersMode();
	_channels = _device->channels();

	switch (_speekersMode)
	{
	case ESpeekersMode_1_0_Mono:
		speekersInfo[0].position.set(0.0f, 1.0f, 0.0f);
		break;
	case ESpeekersMode_2_0_Stereo:
		speekersInfo[0].position.set(-1.0f, 0.0f, 0.0f);
		speekersInfo[1].position.set(1.0f, 0.0f, 0.0f);
		break;
	case ESpeekersMode_2_1_Stereo:
		speekersInfo[0].position.set(-1.0f, 0.0f, 0.0f);
		speekersInfo[1].position.set(1.0f, 0.0f, 0.0f);
		speekersInfo[2].position.set(0.0f, 0.0f, 0.0f);
		break;
	default:
		logError << "unimplemented speekersMode"; 
		break;
	}
	calculateSpeekersInfo();
}

boolean SoundSystem::speekersMode(ESpeekersMode mode)
{
	if(_device && _device->channels() != AudioLib::channelsOfSpeekersMode(mode))
		return false;

	_speekersMode = mode;
	calculateSpeekersInfo();
	return true;
}

void SoundSystem::listenerRadius(float val)
{
    _listenerRadius = val;
	calculateSpeekersInfo();
}
    
void SoundSystem::calculateSpeekersInfo()
{
    for(uint i=0; i < AUDIOLIB_MAX_CHANNELS; ++i)
        speekersInfo[i].calculate(_listenerRadius);
}

void SoundSystem::addMusicWave(const int16* src, uint channels, const float* volume0, const float* volume1, uint pos, uint frames)
{
	if(!src || !frames)
		return;

	if(channels == 2)
	{
		int coef0_l = int(volume0[0] * SOUNDLIB_INT16_TO_INT32_SCALE_FLT);
		int coef1_l = int(volume1[0] * SOUNDLIB_INT16_TO_INT32_SCALE_FLT);
		int coefD_l = (coef1_l - coef0_l) * 65536 / (int)frames;
		int coef_l = coef0_l * 65536;

		int coef0_r = int(volume0[1] * SOUNDLIB_INT16_TO_INT32_SCALE_FLT);
		int coef1_r = int(volume1[1] * SOUNDLIB_INT16_TO_INT32_SCALE_FLT);
		int coefD_r = (coef1_r - coef0_r) * 65536 / (int)frames;
		int coef_r = coef0_r * 65536;

		for(uint i=0; i < frames; ++i, coef_l += coefD_l, coef_r += coefD_r)
		{
			uint c_l = uint(coef_l) >> 16;
			uint c_r = uint(coef_r) >> 16;
			musicSamples[i][0] += src[i*2 + 0] * c_l;
			musicSamples[i][1] += src[i*2 + 1] * c_r;
		}
	}
	else if(channels == 1)
	{
		int coef0 = int(*volume0 * SOUNDLIB_INT16_TO_INT32_SCALE_FLT);
		int coef1 = int(*volume1 * SOUNDLIB_INT16_TO_INT32_SCALE_FLT);
		int coefD = (coef1 - coef0) * 65536 / (int)frames;
		int coef = coef0 * 65536;

		for(uint i=0; i < frames; ++i, coef += coefD)
		{
			uint c = uint(coef) >> 16;
			Assert(c <= 4096);
			int a = src[i] * c;
			musicSamples[i][0] += a;
			musicSamples[i][1] += a;
		}
	}
}




ISoundElement::ISoundElement(SoundSystem& srcSys) : lel(this), renderLel(this), system(srcSys)
{
	system.elementsListLock.lock();
	system.elementsList.insert(lel);
	system.elementsListLock.unlock();
}

void ISoundElement::selfDelete()
{
	renderCallbackActive(false);

	system.elementsListLock.lock();
	system.releasedElementsList.push(this);
	system.elementsListLock.unlock();
}

void ISoundElement::renderCallbackActive(boolean b)
{
	LockGuard lg(system.renderedSourcesLock);
	if(b)
	{
		if(renderLel.included())
			return;

		system.renderedSources.insert(renderLel);
	}
	else
	{
		if(!renderLel.included())
			return;

		renderLel.erase();
	}
}

void SoundSystem::onGenAudioOutput(IAudioOutput& dev)
{
	Assert(&dev == _device);

	renderedSourcesReplyc.clearSaveReserve();
	renderedSourcesLock.lock();
	foreach(e, renderedSources)
		renderedSourcesReplyc.push(&*e);
	renderedSourcesLock.unlock();

	samplesUsed = false;
	memoryClear(samples, _channels * AUDIOLIB_PACKET_FRAMES * sizeof(float));
	memoryClear(&musicSamples[0][0], 2 * AUDIOLIB_PACKET_FRAMES * sizeof(int32));

	_listenerPlaceOld = _listenerPlace;
	_listenerPlace = listenerPlace;

	foreach(e, renderedSourcesReplyc)
		(*e)->onRender();

	if(samplesUsed)
	{
		switch(_device->speekersMode())
		{
		case ESpeekersMode_1_0_Mono:
			for(uint i=0; i < AUDIOLIB_PACKET_FRAMES; ++i)
				_device->samples[i] = fastConvertAndClampToInt16(musicSamples[i][0] + int(samples[i] * (32767.0f * SOUNDLIB_INT16_TO_INT32_SCALE_FLT)));
			break;
		case ESpeekersMode_2_0_Stereo:
			for(uint i=0; i < AUDIOLIB_PACKET_FRAMES; ++i){
				_device->samples[i*2 + 0] = fastConvertAndClampToInt16(musicSamples[i][0] + int(samples[i*2 + 0] * (32767.0f * SOUNDLIB_INT16_TO_INT32_SCALE_FLT)));
				_device->samples[i*2 + 1] = fastConvertAndClampToInt16(musicSamples[i][1] + int(samples[i*2 + 1] * (32767.0f * SOUNDLIB_INT16_TO_INT32_SCALE_FLT)));
			}
			break;
        default:
            Assert(false);
		}
	}
	else
	{
		switch(_device->speekersMode())
		{
		case ESpeekersMode_1_0_Mono:
			for(uint i=0; i < AUDIOLIB_PACKET_FRAMES; ++i)
				_device->samples[i] = fastConvertAndClampToInt16(musicSamples[i][0]);
			break;
		case ESpeekersMode_2_0_Stereo:
			for(uint i=0; i < AUDIOLIB_PACKET_FRAMES; ++i){
				_device->samples[i*2 + 0] = fastConvertAndClampToInt16(musicSamples[i][0]);
				_device->samples[i*2 + 1] = fastConvertAndClampToInt16(musicSamples[i][1]);
			}
			break;
        default:
            Assert(false);
		}
	}

	_callbackMutex.lock();
	if(_callback)
		_callback(*this);
	_callbackMutex.unlock();

	garbadgeWork();
}

void SoundSystem::garbadgeWork()
{
	elementsListLock.lock();

	uint first = 0;
	uint end = releasedElementsList.size();
	while(first < end)
	{
		for(uint i = first; i < end; ++i)
			delete releasedElementsList[i];
		first = end;
		end = releasedElementsList.size();
	}

	releasedElementsList.clearSaveReserve();
	elementsListLock.unlock();
}

}