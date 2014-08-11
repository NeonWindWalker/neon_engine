#include "../../../code/texexp/include.h"

int main(int argc, char** argv)
{
	Base::DefaultLogListener logListener;

	if(argc < 1){
		logError << "no parameters";
		return -1;
	}

	Base::String rootDir = Base::constString(argv[0]);
	while(!Base::isSlesh(*rootDir.last()))
		rootDir.fpop();

	TexExp::doTexExport(rootDir);
	return 0;
}