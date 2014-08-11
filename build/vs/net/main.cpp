#include "../../../code/net/include.h"
#include "../../../code/net/tests/include.h"

int main(int argc, char* argv[])
{
	Base::DefaultLogListener ll;
	NetLib::udpTest();
}
