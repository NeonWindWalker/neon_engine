#include "../../../code/sound/tests/test.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	Base::DefaultLogListener ll;
	SoundLib::testBegin();
	getchar();
	SoundLib::testEnd();
}