#include "../../../code/filestorage/include.h"
#include "../../../code/soundexp/include.h"
#include "../../../code/base/include.h"

int main(int argc, char* argv[])
{
	Base::DefaultLogListener logListener;

	if(argc < 1){
		logError << "no parameters";
		return -1;
	}


	Base::String rootDir = Base::constString(argv[0]);
	while(!Base::isSlesh(*rootDir.last()))
		rootDir.fpop();

	FileStorageLib::IStorage* srcFS = FileStorageLib::openStorage(Base::String() << rootDir << _S("/sources"));
	FileStorageLib::IStorage* destFS = FileStorageLib::openStorage(Base::String() << rootDir << _S("/data"));
	SoundExpLib::doExport(*destFS, *srcFS);
	delete srcFS;
	delete destFS;
	
	return 0;
}