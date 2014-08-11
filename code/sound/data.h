#include "core.h"

struct OpusDecoder;

namespace SoundLib
{

class SoundWavData : public ISoundData
{
public:
	SoundWavData(SoundSystem& sys, ConstBinaryData dat);
	~SoundWavData();

	boolean toOpus(BinaryData& dst);

	const int16* data(uint pos, uint count, boolean needActualFirstFrames);
	
protected:
	int16* _data;
};

class SoundOpusData : public ISoundData
{
public:
	SoundOpusData(SoundSystem& sys, ConstBinaryData dat);
	~SoundOpusData();

	//void toCelt(BinaryData& dst);

	const int16* data(uint pos, uint count, boolean needActualFirstFrames);
	void onRender();

protected:
	byte* _opusData;
	int16* _samples;
	uint samplesDecodingProgress;
	int unusedCount;

	struct Decoder
	{
		OpusDecoder* decoder;
		uint first;
		uint end;
		int unusedCount;
	};

	Array<Decoder> decoders;
};


class SoundOpusStreamedData : public ISoundData
{
public:
	SoundOpusStreamedData(SoundSystem& sys, ConstBinaryData dat);
	~SoundOpusStreamedData();

	const int16* data(uint pos, uint count, boolean needActualFirstFrames);
	void onRender();

protected:
	byte* _opusData;
	
	class Stream
	{
	public:
		OpusDecoder* decoder;
		uint first;
		uint end;
		uint channels;
		int unusedCount;
		static const uint gcSamplesBufferSize = AUDIOLIB_FRAME_RATE * 2 * 2; // 2 sec x stereo
		int16 samples[gcSamplesBufferSize];

		Stream(uint channels);
		~Stream();
		void shiftBuffer();
	};

	Array<Stream*> streams;
};

}