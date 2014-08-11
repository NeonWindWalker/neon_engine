#include "test.h"

namespace AudioLib
{
	
static float phase = 0;

void TestSineVoice_callback(IAudioOutput& dev)
{
    float herts = 300.0f;
    float coef = gcfTwoPi / float(AUDIOLIB_PACKET_FRAMES - 1) * herts * float(AUDIOLIB_PACKET_FRAMES) / float(AUDIOLIB_FRAME_RATE);
    for(uint i=0; i < AUDIOLIB_PACKET_FRAMES; ++i)
    {
        phase += coef;
        if(phase > gcfTwoPi)
            phase -= gcfTwoPi;
		float f = Base::sin(phase);
		for(uint c = 0; c < dev.channels(); c++)
			dev.samples[i * dev.channels() + c] = f * 32767.0f;
        //frame[i] = (i % 2) ? -(1<<23) : (1<<23);
    }
   //dev.frameFltValid = true;
}
    
IAudioOutput* gDevice = 0;
    
void testBegin()
{
    gDevice = IAudioOutput::create();
    gDevice->callback = makeDelegate(&TestSineVoice_callback);
    gDevice->active(true);

	Thread::sleep(3.0f);
	gDevice->active(false);
	Thread::sleep(1.0f);
	gDevice->active(true);
}
    
void testEnd()
{
    delete gDevice;
}
    
}