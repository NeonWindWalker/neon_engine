#pragma once

#include "include.h"

namespace FileStorageLib
{

class Storage : public IStorage
{
private:
	String root;

public:
	Storage(ConstString path);
	~Storage() {}
    
	boolean size(uint64& size, ConstString path);
	boolean exists(ConstString path);
	boolean read(BinaryData& data, ConstString path);
	boolean read(BinaryDataProxy& data, ConstString path, uint64 offset);
	boolean write(ConstString path, ConstBinaryData data);
	boolean append(ConstString path, ConstBinaryData data);
	boolean rewrite(ConstString path, uint64 seek, ConstBinaryData data);
	boolean erase(ConstString path);
    boolean list(Array<String>& files, ConstString path, ConstString basepath);
protected:
    boolean createDirectoryFromLocalFilePath(ConstString filePath);
};

}
