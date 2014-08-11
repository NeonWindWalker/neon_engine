#include "include.h"

#if defined(_WIN32) && !defined(_USING_V110_SDK71_)

#include <stdio.h>
#include <Objbase.h>
//#include<Audioclient.h>
//#include<Mmdeviceapi.h>
#include <Mmreg.h>
#include <MMDeviceAPI.h>
#include <AudioClient.h>
#include <AudioPolicy.h>
//#pragma comment(lib, "ole32.lib")


namespace AudioLib
{

class AudioOutput : public IAudioOutput
{
public:
	void threadFunc(void* p);

	AudioOutput(Base::boolean forceStereo)
	{
		_speekersMode = ESpeekersMode_None;
		_active = false;
		initResult = 0;
		_stopFlag = false;
		memoryClear(frameEx, sizeof(float) * (AUDIOLIB_MAX_CHANNELS * (AUDIOLIB_PACKET_FRAMES + frameExOffset * 2)));
		thread = new Thread(makeDelegate(this, &AudioOutput::threadFunc), &forceStereo);
		while(!initResult)
			Thread::sleep(0.1f);
	}

	~AudioOutput()
	{
		_stopFlag = true;
		delete thread;
	}
		
	void active(boolean01 b) { _active = b; }
	boolean01 active()const { return _active; }

	volatile uint initResult;

protected:
	volatile boolean01 _active;
	volatile boolean01 _stopFlag;
	static const uint frameExOffset = 4;
	DECLARE_ALIGN(16) float frameEx[AUDIOLIB_MAX_CHANNELS * (AUDIOLIB_PACKET_FRAMES + frameExOffset * 2)];
	
	Thread* thread;
	
};

IAudioOutput* IAudioOutput::create(Base::boolean forceStereo)
{
	AudioOutput* p = new AudioOutput(forceStereo);
	if(p->initResult != 1){
		delete p;
		return 0;
	}
	return p;
}


void AudioOutput::threadFunc(void* pbForceStereo)
{
	Base::boolean bForceStereo = *(Base::boolean*)pbForceStereo;
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	const IID IID_IAudioClient = __uuidof(IAudioClient);
	const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

	HRESULT hr;
	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE *pData;
	DWORD flags = 0;

	CoInitialize(NULL);

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);

	if (FAILED(hr)) { goto Exit; }

	hr = pEnumerator->GetDefaultAudioEndpoint(
		eRender, eConsole, &pDevice);
	if (FAILED(hr)) { goto Exit; }

	hr = pDevice->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)&pAudioClient);
	if (FAILED(hr)) { goto Exit; }


	WAVEFORMATEX *pwfx = NULL;
	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) { goto Exit; }
	
	/*
	// set the stream format
	wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	wfx.Format.nChannels = bForceStereo ? 2 : pDefWFX->nChannels;
	wfx.Format.wBitsPerSample = 16;
	wfx.Format.nSamplesPerSec = 48000;
	wfx.Format.nBlockAlign = wfx.Format.nChannels * (wfx.Format.wBitsPerSample / 8);
	wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
	wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;

	wfx.dwChannelMask = bForceStereo ? KSAUDIO_SPEAKER_STEREO : ((WAVEFORMATEXTENSIBLE*)pDefWFX)->dwChannelMask;
	wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
	wfx.Samples.wValidBitsPerSample = 16;
	*/
	
	if(bForceStereo)
	{
		_channels = 2;
		_speekersMode = ESpeekersMode_2_0_Stereo;
		pwfx->nChannels = 2;
		pwfx->nBlockAlign = pwfx->nChannels * (pwfx->wBitsPerSample / 8);
		pwfx->nAvgBytesPerSec = pwfx->nSamplesPerSec * pwfx->nBlockAlign;
		((WAVEFORMATEXTENSIBLE*)pwfx)->dwChannelMask = KSAUDIO_SPEAKER_STEREO;
	}
	else
	{
		switch(((WAVEFORMATEXTENSIBLE*)pwfx)->dwChannelMask)
		{
		case KSAUDIO_SPEAKER_MONO:
			_speekersMode = ESpeekersMode_1_0_Mono;
			break;
		case KSAUDIO_SPEAKER_STEREO:
			_speekersMode = ESpeekersMode_2_0_Stereo;
			break;
		case (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY):
			_speekersMode = ESpeekersMode_2_1_Stereo;
			break;
		case KSAUDIO_SPEAKER_QUAD:
			_speekersMode = ESpeekersMode_4_0_Quad;
			break;
		case KSAUDIO_SPEAKER_SURROUND:
			_speekersMode = ESpeekersMode_4_0_Surround;
			break;
		case KSAUDIO_SPEAKER_QUAD | SPEAKER_LOW_FREQUENCY:
			_speekersMode = ESpeekersMode_4_1_Quad;
			break;
		case KSAUDIO_SPEAKER_SURROUND | SPEAKER_LOW_FREQUENCY:
			_speekersMode = ESpeekersMode_4_1_Surround;
			break;
		case (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT):
			_speekersMode = ESpeekersMode_5_0;
			break;
		case (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT):
			_speekersMode = ESpeekersMode_5_0_Side;
			break;
		case KSAUDIO_SPEAKER_5POINT1:
			_speekersMode = ESpeekersMode_5_1;
			break;
		case KSAUDIO_SPEAKER_5POINT1_SURROUND:
			_speekersMode = ESpeekersMode_5_1_Side;
			break;
		case KSAUDIO_SPEAKER_7POINT1:
			_speekersMode = ESpeekersMode_7_1;
			break;
		case KSAUDIO_SPEAKER_7POINT1_SURROUND:
			_speekersMode = ESpeekersMode_7_1_Side;
			break;
		default:
			logError << "unknow channels"; 
			break;
		}
		_channels = pwfx->nChannels;
	}
	Assert(_channels == channelsOfSpeekersMode(_speekersMode));
	
	hr = pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		10000000, // 1sec
		0,
		pwfx,
		NULL);
	if (FAILED(hr)) { goto Exit; }
	if (pwfx->wBitsPerSample != 32) { goto Exit; }


	// Get the actual size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) { goto Exit; }

	hr = pAudioClient->GetService(
		IID_IAudioRenderClient,
		(void**)&pRenderClient);
	if (FAILED(hr)) { goto Exit; }

	uint packetFramesCount = AUDIOLIB_PACKET_FRAMES * pwfx->nSamplesPerSec / AUDIOLIB_FRAME_RATE;
	initResult = 1;
	bool started = false; 
	while(!_stopFlag)
	{
		if(_active){
			if(!started){
				pAudioClient->Start();
				started = true;
			}
			
		}else{
			if(started){
				pAudioClient->Stop();
				started = false;
			}
			Thread::sleep(0.01f);
			continue;
		}

		hr = pAudioClient->GetCurrentPadding(&numFramesPadding);
		if (FAILED(hr)) { goto Exit; }
		if(numFramesPadding > packetFramesCount)
			Thread::sleep(float(numFramesPadding - packetFramesCount) / (pwfx->nSamplesPerSec * 2));

		// Grab all the available space in the shared buffer.
		hr = pRenderClient->GetBuffer(packetFramesCount, &pData);
		if (FAILED(hr)) { goto Exit; }

		_callbackMutex.lock();
		if(_callback)
		{
			_callback(*this);
			_callbackMutex.unlock();

			for(uint s = 0; s < frameExOffset * 2; ++s)
				for(uint c = 0; c < _channels; ++c)
					frameEx[s * _channels + c] = frameEx[(s + AUDIOLIB_PACKET_FRAMES) * _channels + c];

			for(uint s = frameExOffset * 2; s < AUDIOLIB_PACKET_FRAMES + frameExOffset * 2; ++s)
				for(uint c = 0; c < _channels; ++c)
					frameEx[s * _channels + c] = samples[(s - frameExOffset * 2) * _channels + c] * (1.0f / 32768.0f);
				
			float* dst = (float*)pData;
			for(uint s = 0; s < packetFramesCount; ++s)
			{
				uint n = s * AUDIOLIB_PACKET_FRAMES + AUDIOLIB_PACKET_FRAMES/2 - packetFramesCount/2 + frameExOffset * packetFramesCount;
				uint s2 = n / packetFramesCount;
				float k = float(n % packetFramesCount) / packetFramesCount;
	
				Assert(s2 + 1 < AUDIOLIB_PACKET_FRAMES + frameExOffset * 2);
				for(uint c = 0; c < _channels; ++c)
				{
					dst[s * _channels + c] = frameEx[s2 * _channels + c] * (1.0f - k) + frameEx[(s2 + 1) * _channels + c] * k;
				}
			}
		}
		else
		{
			_callbackMutex.unlock();
			Base::memoryClear(pData, packetFramesCount * pwfx->nChannels * pwfx->wBitsPerSample / 8);
		}
		

		hr = pRenderClient->ReleaseBuffer(packetFramesCount, flags);
		if (FAILED(hr)) { goto Exit; }
	}

	hr = pAudioClient->Stop();  // Stop playing.
	if (FAILED(hr)) { goto Exit; }

Exit:
	initResult = 2;
	CoTaskMemFree(pwfx);
	if(pEnumerator)pEnumerator->Release();
	if(pDevice)pDevice->Release();
	if(pAudioClient)pAudioClient->Release();
	if(pRenderClient)pRenderClient->Release();
}

}

#endif


