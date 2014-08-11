#include "../../../code/filestorage/include.h"
#include "../../../code/meshexp/include.h"
#include "../../../code/reflection/include.h"

int main(int argc, char* argv[])
{
	Base::DefaultLogListener logListener;

	if(argc < 1){
		logError << "no parameters";
		return -1;
	}

	ReflectionLib::initReflectionLibrary();

	Base::String rootDir = Base::constString(argv[0]);
	while(!Base::isSlesh(*rootDir.last()))
		rootDir.fpop();

	FileStorageLib::IStorage* srcFS = FileStorageLib::openStorage(rootDir);
	FileStorageLib::IStorage* destFS = FileStorageLib::openStorage(Base::String() << rootDir << _S("/data"));
	MeshExpLib::doExport(*destFS, *srcFS);
	delete srcFS;
	delete destFS;
	ReflectionLib::deinitReflectionLibrary();
	return 0;
}