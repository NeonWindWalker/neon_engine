#pragma  once

#include "core.h"

namespace SoundLib
{

static INLINE float smoothVolumeValue(float volume, float smoothTime, float dt, float targetValue)
{
	return volume + (targetValue - volume) * (1.0f - Base::exp(- dt / smoothTime));
}

struct WAVFileHeader
{
	char sGroupID[4];
	uint32 dwFileLength;
	char sRiffType[4];

	WAVFileHeader()
	{
		sGroupID[0] = 'R';
		sGroupID[1] = 'I';
		sGroupID[2] = 'F';
		sGroupID[3] = 'F';
		dwFileLength = 0;
		sRiffType[0] = 'W';
		sRiffType[1] = 'A';
		sRiffType[2] = 'V';
		sRiffType[3] = 'E';
	}

	void load(EndianSafeConstBinaryDataAccessor& a)
	{
		a >> sGroupID[0] >> sGroupID[1] >> sGroupID[2] >> sGroupID[3];
		a >> dwFileLength;
		a >> sRiffType[0] >> sRiffType[1] >> sRiffType[2] >> sRiffType[3];
	}

	void save(EndianSafeBinaryDataAccessor& a)
	{
		a << sGroupID[0] << sGroupID[1] << sGroupID[2] << sGroupID[3];
		a << dwFileLength;
		a << sRiffType[0] << sRiffType[1] << sRiffType[2] << sRiffType[3];
	}

	boolean isValid()
	{
		return sGroupID[0] == 'R' &&
			sGroupID[1] == 'I' &&
			sGroupID[2] == 'F' &&
			sGroupID[3] == 'F' &&
			sRiffType[0] == 'W' &&
			sRiffType[1] == 'A' &&
			sRiffType[2] == 'V' &&
			sRiffType[3] == 'E';
	}
};

struct WAVFileFormatChunk
{
	char sGroupID[4];
	uint32 dwChunkSize;
	uint16 wFormatTag;
	uint16 wChannels;
	uint32 dwSamplesPerSec;
	uint32 dwAvgBytesPerSec;
	uint16 wBlockAlign;
	uint16 dwBitsPerSample;

	WAVFileFormatChunk()
	{
		sGroupID[0] = 'f';
		sGroupID[1] = 'm';
		sGroupID[2] = 't';
		sGroupID[3] = ' ';
		dwChunkSize = sizeof(WAVFileFormatChunk) - 8;
		wFormatTag = 1;
		wChannels = 1;
		dwSamplesPerSec = 0;
		dwAvgBytesPerSec = 0;
		wBlockAlign = 0;
		dwBitsPerSample = 0;
	}

	void load(EndianSafeConstBinaryDataAccessor& a)
	{
		a >> sGroupID[0] >> sGroupID[1] >> sGroupID[2] >> sGroupID[3];
		a >> dwChunkSize >> wFormatTag >> wChannels >> dwSamplesPerSec >> dwAvgBytesPerSec >> wBlockAlign >> dwBitsPerSample;
		a.dseek(dwChunkSize + 8 - sizeof(WAVFileFormatChunk));
	}

	void save(EndianSafeBinaryDataAccessor& a)
	{
		a << sGroupID[0] << sGroupID[1] << sGroupID[2] << sGroupID[3];
		a << dwChunkSize << wFormatTag << wChannels << dwSamplesPerSec << dwAvgBytesPerSec << wBlockAlign << dwBitsPerSample;
	}

	void set(uint channels, uint freq = AUDIOLIB_FRAME_RATE)
	{
		wChannels = channels;
		dwSamplesPerSec = freq;
		dwBitsPerSample = 16;
		wBlockAlign = channels * sizeof(uint16);
		dwAvgBytesPerSec = freq * wBlockAlign;
	}

	boolean isValid()
	{
		return sGroupID[0] == 'f' &&
			sGroupID[1] == 'm' &&
			sGroupID[2] == 't' &&
			sGroupID[3] == ' ';
	}
};

struct WAVFileDataChunk
{
	char sGroupID[4];
	uint32 dwChunkSize;

	WAVFileDataChunk()
	{
		sGroupID[0] = 'd';
		sGroupID[1] = 'a';
		sGroupID[2] = 't';
		sGroupID[3] = 'a';
		dwChunkSize = 0;
	}

	void load(EndianSafeConstBinaryDataAccessor& a)
	{
		a >> sGroupID[0] >> sGroupID[1] >> sGroupID[2] >> sGroupID[3];
		a >> dwChunkSize;
	}

	void save(EndianSafeBinaryDataAccessor& a)
	{
		a << sGroupID[0] << sGroupID[1] << sGroupID[2] << sGroupID[3];
		a << dwChunkSize;
	}

	boolean isValid()
	{
		return sGroupID[0] == 'd' &&
			sGroupID[1] == 'a' &&
			sGroupID[2] == 't' &&
			sGroupID[3] == 'a';
	}
};


struct MyOpusFileHeader
{
	uint32 framesPerSecond;
	uint32 channels;
	uint32 packetFrames;
	uint32 packetSize;

	MyOpusFileHeader()
	{
		framesPerSecond = AUDIOLIB_FRAME_RATE;
		channels = 1;
		packetFrames = SOUNDLIB_OPUS_PACKET_FRAMES;
		packetSize = SOUNDLIB_OPUS_PACKET_SIZE;
	}

	void load(EndianSafeConstBinaryDataAccessor& a)
	{
		a >> framesPerSecond >> channels >> packetFrames >> packetSize;
	}

	void save(EndianSafeBinaryDataAccessor& a)
	{
		a << framesPerSecond << channels << packetFrames << packetSize;
	}
};

void celtCompress(BinaryData& dst, ConstBinaryData src);

}