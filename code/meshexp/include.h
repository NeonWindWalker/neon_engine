#include "../base/include.h"
#include "../filestorage/include.h"

namespace MeshExpLib
{
USING_BASE_LIB

extern boolean doExport(BinaryData& meshFile, BinaryData& collisionFile, ConstBinaryData src);
extern boolean doExport(FileStorageLib::IStorage& destFS, FileStorageLib::IStorage& srcFS);

}