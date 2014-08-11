#include "test.h"
#include "../../filestorage/include.h"

namespace SoundLib
{
	

IAudioOutput* gOutput = 0;
SoundSystem* gSystem = 0;

class SineSoundSource : public ISoundSource
{
public:
	SineSoundSource() : ISoundSource(*gSystem)
	{
		phase = 0;
		//active(true);
	}

	float phase;

	void onRender()
	{
		float herts = 300.0f;
		float coef = gcfTwoPi / float(AUDIOLIB_PACKET_FRAMES - 1) * herts * float(AUDIOLIB_PACKET_FRAMES) / float(AUDIOLIB_FRAME_RATE);
		int16 data[AUDIOLIB_PACKET_FRAMES];
		for(uint i=0; i < AUDIOLIB_PACKET_FRAMES; ++i)
		{
			phase += coef;
			if(phase > gcfTwoPi)
				phase -= gcfTwoPi;
			data[i] = int(Base::sin(phase) * 32767.0f);
		}
		system.addMusicWave(data, 1);
	}
};

SineSoundSource* gSineSource = 0;
ISoundData* gData = 0;
SoundMusicSource* gMusic = 0;
    
void testBegin()
{
	gOutput = IAudioOutput::create();
    gSystem = new SoundSystem();
	gSystem->attach(gOutput);
    gSystem->device()->active(true);

	gSineSource = new SineSoundSource();
	//gSineSource->active(true);

	BinaryData dat;
	StrongPtr<FileStorageLib::IStorage> storage = FileStorageLib::openStorage(_CS(""));

	/*
	BinaryData dat2;
	storage->read(dat2, _CS("sound.wav"));
	SoundWavData* gData2 = new SoundWavData(*gSystem, dat2);
	BinaryData opusData;
	gData2->toOpus(opusData);
	storage->write(_CS("sound.opus"), opusData);
	gData2->selfDelete();
	*/

	storage->read(dat, _CS("sound.opus"));

	//gData = new SoundWavData(*gSystem, dat);
	gData = new SoundOpusStreamedData(*gSystem, dat);
	gMusic = new SoundMusicSource(*gSystem, *gData);
	gMusic->setLooped();
	gMusic->active(true);	

}
    
void testEnd()
{
	delete gSystem;
    delete gOutput;
}
    
}