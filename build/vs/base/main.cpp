#include "../../../code/base/include.h"
#include "../../../code/base/tests/tests.h"

void breakpoint_trigging_fix_extern_function()
{
	return;
}

int main(int argc, char* argv[])
{
	Base::DefaultLogListener ll;
	Base::testAll();
}
