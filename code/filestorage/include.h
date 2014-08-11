#pragma once

#include "../base/string.h"
#include "../base/array.h"
#include "../base/smartPointers.h"
#include "../base/bindata.h"

namespace FileStorageLib 
{

USING_BASE_LIB

struct FileChangedNotification
{
	String name;
	boolean exist;
};

class IStorage : public RefCountable
{
public:
	virtual ~IStorage(){}

	virtual boolean size(uint64& size, ConstString path)=0;
	virtual boolean exists(ConstString path){ uint64 s; return size(s, path); }
	virtual boolean read(BinaryData& data, ConstString path)=0;
	virtual boolean read(BinaryDataProxy& data, ConstString path, uint64 offset)=0;
	virtual boolean write(ConstString path, ConstBinaryData data)=0;
	virtual boolean append(ConstString path, ConstBinaryData data)=0;
	virtual boolean rewrite(ConstString path, uint64 offset, ConstBinaryData data)=0;
	virtual boolean erase(ConstString path)=0;
	virtual boolean list(Array<String>& files, ConstString path, ConstString basepath = ConstString())=0;

	virtual uint ladleOutChangeEvents(Array<FileChangedNotification>& events) { return 0; }
	virtual boolean watching(boolean on) { return false; }
	virtual boolean watching() { return false; }

	virtual boolean execute(ConstString workdir, ConstString path, ConstString params, boolean wait = true) { return false; }
};

extern IStorage* openStorage(ConstString rootPath);

#ifdef __ANDROID__
extern IStorage* openAssetStorage(void* jVM, void* jAssetReader);
#endif

}


