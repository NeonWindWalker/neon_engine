#include "data.h"
#include "system.h"
#include "utils.h"
#include "../../../external/opus/include/opus.h"

namespace SoundLib
{

SoundWavData::SoundWavData(SoundSystem& sys, ConstBinaryData dat) : ISoundData(sys)
{
	_data = 0;
	_channels = 0;
	_frames = 0;

	if(dat.byteSize() < sizeof(WAVFileHeader) + sizeof(WAVFileFormatChunk))
		return;
	EndianSafeConstBinaryDataAccessor a(dat);

	WAVFileHeader fh;
	fh.load(a);
	if(!fh.isValid())
		return;

	WAVFileFormatChunk ffc;
	ffc.load(a);
	if(!ffc.isValid())
		return;

	WAVFileDataChunk fdc;
	fdc.load(a);
	if(!fdc.isValid())
		return;

	_channels = ffc.wChannels;
	_frames = fdc.dwChunkSize / (_channels * sizeof(uint16));
	_data = new int16 [_channels * _frames];
	memoryCopy(_data, a.seek() + (const byte*)dat.first(), fdc.dwChunkSize);
}

SoundWavData::~SoundWavData()
{
	delete _data;
}

const int16* SoundWavData::data(uint pos, uint count, boolean needActualFirstFrames)
{
	if(pos + count <= _frames)
		return _data + pos * _channels;
	return 0;
}

boolean SoundWavData::toOpus(BinaryData& dst)
{
	EndianSafeBinaryDataAccessor a(dst);

	MyOpusFileHeader header;
	header.channels = _channels;
	header.packetSize = SOUNDLIB_OPUS_PACKET_SIZE * _channels;
	header.save(a);

	int err;
	OpusEncoder* enc = opus_encoder_create(header.framesPerSecond, header.channels, OPUS_APPLICATION_AUDIO, &err);
	if(!enc){
		logError << "opus_encoder_create ";
		return false;
	}

	uint bitrate = AUDIOLIB_FRAME_RATE / SOUNDLIB_OPUS_PACKET_FRAMES * 8 * _channels * SOUNDLIB_OPUS_PACKET_SIZE;
	err = opus_encoder_ctl(enc, OPUS_SET_BITRATE(bitrate));
	if(err != OPUS_OK){
		logError << "opus_encoder_ctl OPUS_SET_BITRATE " << bitrate;
	}
		
	byte buff[AUDIOLIB_PACKET_FRAMES * AUDIOLIB_MAX_CHANNELS * 2];
	Assert(AUDIOLIB_PACKET_FRAMES * AUDIOLIB_MAX_CHANNELS * 2 > header.packetSize);
	uint unusedbytes = 0;
	for(uint fr=0; fr + header.packetFrames <= _frames; fr += header.packetFrames)
	{
		int s = opus_encode(enc, _data + _channels * fr, header.packetFrames, buff, header.packetSize);
		Assert(s > 0);

		int d = header.packetSize - s;
		if(d){
			memoryClear(buff + s, d);
			unusedbytes += d; 
		}
			
		a.write(buff, header.packetSize);
	}
	opus_encoder_destroy(enc);
	logInfo << "unused bytes " << unusedbytes;
	return true;
}


SoundOpusData::SoundOpusData(SoundSystem& sys, ConstBinaryData dat) : ISoundData(sys)
{
	_opusData = 0;
	_samples = 0;
	_channels = 0;
	_frames = 0;
	samplesDecodingProgress = 0;

	if(dat.byteSize() < sizeof(MyOpusFileHeader))
		return;
	EndianSafeConstBinaryDataAccessor a(dat);

	MyOpusFileHeader fh;
	fh.load(a);

	_channels = fh.channels;
	uint s = dat.byteSize() - a.seek();
	_frames = (s / fh.packetSize) * fh.packetFrames;
	_opusData = new byte [s];
	memoryCopy(_opusData, a.seek() + (const byte*)dat.first(), s);

	renderCallbackActive(true);
}

SoundOpusData::~SoundOpusData()
{
	delete _opusData;
	delete _samples;
	foreach(dec, decoders)
		opus_decoder_destroy(dec->decoder);
}

const int16* SoundOpusData::data(uint pos, uint count, boolean needActualFirstFrames)
{
	if(pos + count > _frames)
		return 0;

	unusedCount = 0;

	if(_samples && pos + count <= samplesDecodingProgress)
		return _samples + pos * _channels;

	if(!_samples){
		_samples = new int16[_frames * _channels];
		samplesDecodingProgress = 0;
	}
		
	Decoder* dec = 0;
	foreach(e, decoders)
	{
		if(pos >= e->first && pos <= e->end)
		{
			dec = e;
			break;
		}
	}

	if(!dec)
	{
		dec = decoders.push();
		int err;
		dec->decoder = opus_decoder_create(AUDIOLIB_FRAME_RATE, _channels, &err);
		dec->first = (pos / SOUNDLIB_OPUS_PACKET_FRAMES) * SOUNDLIB_OPUS_PACKET_FRAMES;
		dec->end = dec->first;
		dec->unusedCount = 0;

		/*Timer timer;
		for(uint n=0; n < 100; n++)
		for(uint packet = 0; packet < _frames / 960; packet ++)
			opus_decode(dec->decoder, _opusData + 120 * _channels * packet, 120, _samples + _channels * packet * 960, 960, 0);
		dec->end = _frames;
		float t = timer.dt();
		logInfo << "time " << t << " speed " << (_frames / AUDIOLIB_FRAME_RATE * 100) / t;
		*/

		//for(uint packet = 0; packet < _frames / SOUNDLIB_OPUS_PACKET_FRAMES; packet ++)
		//	opus_decode(dec->decoder, _opusData + SOUNDLIB_OPUS_PACKET_SIZE * _channels * packet, SOUNDLIB_OPUS_PACKET_SIZE, _samples + _channels * packet * SOUNDLIB_OPUS_PACKET_FRAMES, SOUNDLIB_OPUS_PACKET_FRAMES, 0);
		//dec->end = _frames;
	}

	if(pos + count > dec->end)
	{
		uint end_packet = (pos + count + SOUNDLIB_OPUS_PACKET_FRAMES - 1) / SOUNDLIB_OPUS_PACKET_FRAMES;
		for(uint packet = dec->end / SOUNDLIB_OPUS_PACKET_FRAMES; packet < end_packet; packet ++)
			Assert(SOUNDLIB_OPUS_PACKET_FRAMES == opus_decode(dec->decoder, _opusData + SOUNDLIB_OPUS_PACKET_SIZE * _channels * packet, SOUNDLIB_OPUS_PACKET_SIZE * _channels, _samples + _channels * packet * SOUNDLIB_OPUS_PACKET_FRAMES, SOUNDLIB_OPUS_PACKET_FRAMES, 0));

		dec->end = end_packet * SOUNDLIB_OPUS_PACKET_FRAMES;
		Assert(pos + count <= dec->end);
	}

	dec->unusedCount = 0;
	return _samples + pos * _channels;
}

void SoundOpusData::onRender()
{
	if(_samples && samplesDecodingProgress != _frames)
	{
		Decoder* dec = 0;
		foreach(e, decoders)
		{
			if(e->first == 0)
			{
				dec = e;
				break;
			}
		}

		if(!dec)
		{
			dec = decoders.push();
			int err;
			dec->decoder = opus_decoder_create(AUDIOLIB_FRAME_RATE, _channels, &err);
			dec->first = 0;
			dec->end = 0;
		}

		if(dec->end == samplesDecodingProgress)
		{
			uint packet = dec->end / SOUNDLIB_OPUS_PACKET_FRAMES;
			opus_decode(dec->decoder, _opusData + SOUNDLIB_OPUS_PACKET_SIZE * _channels * packet, SOUNDLIB_OPUS_PACKET_SIZE * _channels, _samples + _channels * packet * SOUNDLIB_OPUS_PACKET_FRAMES, SOUNDLIB_OPUS_PACKET_FRAMES, 0);
			dec->end += SOUNDLIB_OPUS_PACKET_FRAMES;
			dec->unusedCount = 0;
		}
		samplesDecodingProgress = dec->end;
	}

	irforeach(i, decoders)
	{
		if( ++ decoders[i].unusedCount >= 5)
		{
			opus_decoder_destroy(decoders[i].decoder);
			decoders.erase(i);
		}
	}

	if(++ unusedCount >= AUDIOLIB_FRAME_RATE / AUDIOLIB_PACKET_FRAMES / 2)
	{
		delete _samples;
		_samples = 0;
		samplesDecodingProgress = 0;
	}
}


SoundOpusStreamedData::SoundOpusStreamedData(SoundSystem& sys, ConstBinaryData dat) : ISoundData(sys)
{
	_opusData = 0;
	_channels = 0;
	_frames = 0;

	if(dat.byteSize() < sizeof(MyOpusFileHeader))
		return;
	EndianSafeConstBinaryDataAccessor a(dat);

	MyOpusFileHeader fh;
	fh.load(a);

	_channels = fh.channels;
	uint s = dat.byteSize() - a.seek();
	_frames = (s / fh.packetSize) * fh.packetFrames;
	_opusData = new byte [s];
	memoryCopy(_opusData, a.seek() + (const byte*)dat.first(), s);

	renderCallbackActive(true);
}

SoundOpusStreamedData::~SoundOpusStreamedData()
{
	delete _opusData;
	foreach(p, streams)
		delete *p;
}

const int16* SoundOpusStreamedData::data(uint pos, uint count, boolean needActualFirstFrames)
{
	if(pos + count > _frames || count > Stream::gcSamplesBufferSize / 4 / _channels)
		return 0;

	Stream* stream = 0;
	foreach(e, streams)
	{
		if(pos >= (*e)->first && pos <= (*e)->end)
		{
			stream = *e;
			break;
		}
	}

	if(!stream)
	{
		stream = new Stream(_channels);
		streams.push(stream);

		stream->first = (pos / SOUNDLIB_OPUS_PACKET_FRAMES) * SOUNDLIB_OPUS_PACKET_FRAMES;
		stream->end = stream->first;
	}

	if(pos + count > stream->end)
	{
		uint end_packet = (pos + count + SOUNDLIB_OPUS_PACKET_FRAMES - 1) / SOUNDLIB_OPUS_PACKET_FRAMES;
		for(uint packet = stream->end / SOUNDLIB_OPUS_PACKET_FRAMES; packet < end_packet; packet ++)
		{
			int count = opus_decode(stream->decoder, _opusData + SOUNDLIB_OPUS_PACKET_SIZE * _channels * packet, SOUNDLIB_OPUS_PACKET_SIZE * _channels, stream->samples + _channels * (packet * SOUNDLIB_OPUS_PACKET_FRAMES - stream->first), SOUNDLIB_OPUS_PACKET_FRAMES, 0);
			Assert(count > 0);
		}
			

		stream->end = end_packet * SOUNDLIB_OPUS_PACKET_FRAMES;
		Assert(pos + count <= stream->end);
	}

	stream->unusedCount = 0;
	return stream->samples + (pos - stream->first) * _channels;
}

void SoundOpusStreamedData::onRender()
{
	irforeach(i, streams)
	{
		if( ++ streams[i]->unusedCount >= 5)
		{
			delete streams[i];
			streams.erase(i);
		}
		else streams[i]->shiftBuffer();
	}
}


SoundOpusStreamedData::Stream::Stream(uint channels)
{
	this->channels = channels;
	int err;
	decoder = opus_decoder_create(AUDIOLIB_FRAME_RATE, channels, &err);
	first = 0;
	end = 0;
	unusedCount = 0;
}

SoundOpusStreamedData::Stream::~Stream()
{
	if(decoder)
		opus_decoder_destroy(decoder);
}

void SoundOpusStreamedData::Stream::shiftBuffer()
{
	uint d = Stream::gcSamplesBufferSize / 4 / channels;
	if(d * 4 - (end - first) < d)
	{
		memoryCopy(samples, samples + d * channels, d * channels * sizeof(int16));
		memoryCopy(samples + d * channels, samples + d * channels * 2, (end - first - d*2) * channels * sizeof(int16));
		first += d;
	}
}

}