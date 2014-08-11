#include "include.h"

#if defined(_WIN32) && defined(_USING_V110_SDK71_)

namespace AudioLib
{

IAudioOutput* IAudioOutput::create(Base::boolean forceStereo)
{
	return 0;
}

}

#endif


