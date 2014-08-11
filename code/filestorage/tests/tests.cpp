#include "../include.h"
#include "tests.h"
#include "../../base/include.h"

namespace FileStorageLib 
{

void testFileSystem()
{
	StrongPtr<IStorage> storage = openStorage(_CS(""));
	Array<Base::String> dirs,files;
	storage->list(files, _CS(""));
	for(Base::uint i=0; i<dirs.size(); ++i)
		logInfoSkim << dirs[i];
	for(Base::uint i=0; i<files.size(); ++i)
		logInfoSkim << files[i];

	BinaryData data;
	storage->read(data, _CS("x.txt"));
	storage->write(_CS("x_copy.txt"), data);
}

}