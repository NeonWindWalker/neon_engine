#include "include.h"

#ifdef __APPLE__

#include <AudioToolbox/AudioToolbox.h>

extern "C" OSStatus MyAudioOutputCallback(void * inRefCon,
                                          AudioUnitRenderActionFlags * ioActionFlags,
                                          const AudioTimeStamp * inTimeStamp,
                                          UInt32 inBusNumber,
                                          UInt32 inNumberFrames,
                                          AudioBufferList * ioData);

namespace AudioLib
{

class IOSAudioOutput : public IAudioOutput
{
public:
    boolean01 _play;
    AudioUnit outputUnit;
    int16 saveBuffer[2 * AUDIOLIB_PACKET_FRAMES];
    uint savedFramesCount;
    uint savedFramesOffset;
    boolean _isMediaPlayerPlaying;
    uint _frameNumber;
    
    
    IOSAudioOutput()
    {
        _play = false;
        _channels = 2;
        _speekersMode = ESpeekersMode_2_0_Stereo;
        _isMediaPlayerPlaying = false;
        _frameNumber = 0;
        
        //  First, we need to establish which Audio Unit we want.
        
        //  We start with its description, which is:
        AudioComponentDescription outputUnitDescription;
        memset(&outputUnitDescription, 0, sizeof(outputUnitDescription));
        outputUnitDescription.componentType         = kAudioUnitType_Output;
        outputUnitDescription.componentSubType      = kAudioUnitSubType_RemoteIO;
        outputUnitDescription.componentManufacturer = kAudioUnitManufacturer_Apple;
        
        //  Next, we get the first (and only) component corresponding to that description
        AudioComponent outputComponent = AudioComponentFindNext(NULL, &outputUnitDescription);
        Assert(outputComponent);
        
        //  Now we can create an instance of that component, which will create an
        //  instance of the Audio Unit we're looking for (the default output)
        AudioComponentInstanceNew(outputComponent, &outputUnit);
        
        Assert(outputUnit);
        
        //  Next we'll tell the output unit what format our generated audio will
        //  be in. Generally speaking, you'll want to stick to sane formats, since
        //  the output unit won't accept every single possible stream format.
        //  Here, we're specifying floating point samples with a sample rate of
        //  44100 Hz in mono (i.e. 1 channel)
        AudioStreamBasicDescription ASBD;
        memset(&ASBD, 0, sizeof(ASBD));
        
        ASBD.mSampleRate       = AUDIOLIB_FRAME_RATE;
        ASBD.mFormatID         = kAudioFormatLinearPCM;
        ASBD.mFormatFlags      = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked;
        ASBD.mChannelsPerFrame = 2;
        ASBD.mFramesPerPacket  = 1;
        ASBD.mBitsPerChannel   = 16;
        ASBD.mBytesPerPacket   = 4;
        ASBD.mBytesPerFrame    = 4;
        
        
        AudioUnitSetProperty(outputUnit,
                             kAudioUnitProperty_StreamFormat,
                             kAudioUnitScope_Input,
                             0,
                             &ASBD,
                             sizeof(ASBD));
        
        //  Next step is to tell our output unit which function we'd like it
        //  to call to get audio samples. We'll also pass in a context pointer,
        //  which can be a pointer to anything you need to maintain state between
        //  render callbacks. We only need to point to a double which represents
        //  the current phase of the sine wave we're creating.
        AURenderCallbackStruct callbackInfo;
        memset(&callbackInfo, 0, sizeof(callbackInfo));
        callbackInfo.inputProc       = &MyAudioOutputCallback;
        callbackInfo.inputProcRefCon = this;
        
        AudioUnitSetProperty(outputUnit,
                             kAudioUnitProperty_SetRenderCallback,
                             kAudioUnitScope_Input,
                             0,
                             &callbackInfo,
                             sizeof(callbackInfo));


		UInt32 sessionCategory = kAudioSessionCategory_AmbientSound;
		AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof (sessionCategory), &sessionCategory);
        
        //  Here we're telling the output unit to start requesting audio samples
        //  from our render callback. This is the line of code that starts actually
        //  sending audio to your speakers.
        AudioUnitInitialize(outputUnit);
        //AudioOutputUnitStart(outputUnit);
    }
    
    ~IOSAudioOutput()
    {
        active(false);
        AudioOutputUnitStop(outputUnit);
        AudioUnitUninitialize(outputUnit);
        AudioComponentInstanceDispose(outputUnit);
    }
    
    virtual void active(boolean01 b)
    {
        if(_play == b)
            return;
        
        if(b)
        {
            OSStatus err = AudioOutputUnitStart(outputUnit);
            if (err != noErr)
                logError << "AudioQueueStart() error: " << print((uint)err);
        }
        else
        {
            OSStatus err = AudioOutputUnitStop(outputUnit);
            if (err != noErr)
                logError << "AudioQueueStop() error: " << print((uint)err);
        }
        
        _play = b;
    }
    
    virtual boolean01 active()const
    {
        return _play;
    }
    
    
    virtual boolean01 isMediaPlayerPlaying()const
    {
        return _isMediaPlayerPlaying;
    }
    
    void doWork(
                UInt32 inNumberFrames,
                AudioBufferList * ioData)
    {
        if(_frameNumber % 32 == 0)
        {
            UInt32 otherAudioIsPlaying = false;
            UInt32 otherAudioIsPlayingSize = sizeof(otherAudioIsPlaying);
            AudioSessionGetProperty(kAudioSessionProperty_OtherAudioIsPlaying, &otherAudioIsPlayingSize, &otherAudioIsPlaying);
            if(_isMediaPlayerPlaying != otherAudioIsPlaying)
            {
                 _isMediaPlayerPlaying = otherAudioIsPlaying;
                logInfo << "otherAudioIsPlaying " << print(_isMediaPlayerPlaying);
            }
           
        }
        _frameNumber ++;
        
        
        uint S = _channels * sizeof(int16);
        uint needFrames = inNumberFrames;
        uint frameOffset = 0;
        
        if(savedFramesCount)
        {
            if(savedFramesCount >= needFrames)
            {
                memcpy(ioData->mBuffers[0].mData, saveBuffer, needFrames * S);
                savedFramesOffset += needFrames;
                savedFramesCount -= needFrames;
                return;
            }
            else
            {
                memcpy(ioData->mBuffers[0].mData, saveBuffer, savedFramesCount * S);
                frameOffset += savedFramesCount;
                needFrames -= savedFramesCount;
                savedFramesCount = 0;
                savedFramesOffset = 0;
            }
        }
        
        while(needFrames)
        {
            if(_play)
            {
                _callbackMutex.lock();
                if(_callback)
                    _callback(*this);
                _callbackMutex.unlock();
            }
            else
                memset(samples, 0, AUDIOLIB_PACKET_FRAMES * S);
            
            uint tosend = minimum<uint>(AUDIOLIB_PACKET_FRAMES, needFrames);
            uint tosave = AUDIOLIB_PACKET_FRAMES - tosend;
            
            memcpy((byte*)ioData->mBuffers[0].mData + frameOffset * S, samples, tosend * S);
            frameOffset += tosend;
            
            if(tosave)
            {
                memcpy(saveBuffer, (byte*)samples + tosend * S, tosave * S);
                savedFramesCount = tosave;
                savedFramesOffset = 0;
            }
            
            needFrames -= tosend;
        }
    }
};

    
IAudioOutput* IAudioOutput::create(boolean bFoceStereo)
{
    return new IOSAudioOutput();
}
    
}

OSStatus MyAudioOutputCallback(void * inRefCon,
                               AudioUnitRenderActionFlags * ioActionFlags,
                               const AudioTimeStamp * inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList * ioData)
{
    ((AudioLib::IOSAudioOutput*)inRefCon)->doWork(inNumberFrames, ioData);
    return noErr;
}

#endif
