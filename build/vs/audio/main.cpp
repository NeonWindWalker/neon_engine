#include "../../../code/audio/tests/test.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	Base::DefaultLogListener ll;
	AudioLib::testBegin();
	getchar();
	AudioLib::testEnd();
}