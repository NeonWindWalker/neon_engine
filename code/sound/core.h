#pragma once

#include "../base/include.h"
#include "../audio/include.h"

namespace SoundLib
{

USING_BASE_LIB
using namespace AudioLib;

#define SOUNDLIB_OPUS_PACKET_FRAMES 960
#define SOUNDLIB_OPUS_PACKET_SIZE 160
//64 kbit / channel



class SoundSystem;

class ISoundElement : public RefCountable
{
public:
	SoundSystem& system;

	ISoundElement(SoundSystem& srcSystem);
	virtual void selfDelete();
	virtual void onRender(){}

protected:
	ListElement<ISoundElement> lel;
	ListElement<ISoundElement> renderLel;

	boolean renderCallbackActive() const { return renderLel.included(); }
	void renderCallbackActive(boolean b);	

	virtual ~ISoundElement(){}
	friend class SoundSystem;
};

class ISoundData : public ISoundElement
{
public:
	ISoundData(SoundSystem& srcDevice) : ISoundElement(srcDevice)
	{}

	uint frames()const { return _frames; }
	uint channels()const { return _channels; }
	virtual const int16* data(uint pos, uint count, boolean needActualFirstFrames)=0;

protected:
	uint _frames;
	uint _channels;
};

class ISoundSource : public ISoundElement
{
public:
	ISoundSource(SoundSystem& srcDevice) : ISoundElement(srcDevice)
	{}
};

}