#include "include.h"
#include "stdio.h"
#include "../base/scalar.h"
#include "../base/log.h"

#include <stdio.h>

namespace FileStorageLib
{
	boolean Storage::size(uint64& size, ConstString path)
	{
		String fullpath = root;
		fullpath << path;
		FILE* handle = fopen(fullpath.c_str(), "rb");
		if(handle == 0){
			logError << "cant fopen file = \"" << path << "\"";
			return false;
		}

		fseek(handle, 0, SEEK_END);
		size = ftell(handle);
		fclose(handle);
		return true;
	}

	boolean Storage::exists(ConstString path)
	{
		String fullpath = root;
		fullpath << path;
		FILE* handle = fopen(fullpath.c_str(), "rb");
		if(handle == 0)
			return false;

		fclose(handle);
		return true;
	}

	boolean Storage::read(BinaryData& data, ConstString path)
	{
		String fullpath = root;
		fullpath << path;
		FILE* handle = fopen(fullpath.c_str(), "rb");
		if(handle == 0){
			logError << "cant fopen file = \"" << path << "\"";
			return false;
		}

		fseek(handle, 0, SEEK_END);
		uint64 size = ftell(handle);
		fseek(handle, 0, SEEK_SET);

		data.resize(size);

		uint64 readed = fread(data.first(), 1, data.byteSize(), handle);
		fclose(handle);
		if(readed == data.byteSize())
			return true;
		logError << "read " << data.byteSize() <<"bytes from file = \"" << fullpath << "\"";
		return false;
	}

	boolean Storage::read(BinaryDataProxy& data, ConstString path, uint64 offset)
	{
		String fullpath = root;
		fullpath << path;
		FILE* handle = fopen(fullpath.c_str(), "rb");
		if(handle == 0){
			logError << "cant fopen file = \"" << path << "\"";
			return false;
		}

		if(!fseek(handle, offset, SEEK_SET)){
			logError << "seek to " << offset <<" file = \"" << fullpath << "\"";
			return false;
		}

		uint64 readed = fread(data.first(), 1, data.byteSize(), handle);
		fclose(handle);
		if(readed == data.byteSize())
			return true;
		logError << "read " << data.byteSize() <<"bytes from file = \"" << fullpath << "\"";
		return false;
	}

	boolean Storage::write(ConstString path, ConstBinaryData data)
	{
        logInfo << "statring write";
        
		if(!createDirectoryFromLocalFilePath(path))
			return false;
        
        logInfo << "continue write";

		String fullpath = root;
		fullpath << path;
		FILE* handle = fopen(fullpath.c_str(), "wb");
		if(handle == 0){
			logError << "cant fopen " << path << _CS(" full path ") << fullpath;
			return 0;
		}
		
		uint64 writed = fwrite(data.first(), 1, data.byteSize(), handle);
		fclose(handle);
		if(writed == data.byteSize()){
            logInfo << "write " << data.byteSize() <<"bytes to file = \"" << fullpath << "\"";
			return true;
        }
		logError << "write " << data.byteSize() <<"bytes to file = \"" << fullpath << "\"";
		return false;

	}

	boolean Storage::append(ConstString path, ConstBinaryData data)
	{
		String fullpath = root;
		fullpath << path;
		FILE* handle = fopen(fullpath.c_str(), "ab");
		if(handle == 0){
			logError << "cant fopen " << path;
			return 0;
		}

		uint64 writed = fwrite(data.first(), 1, data.byteSize(), handle);
		fclose(handle);
		if(writed == data.byteSize())
			return true;
		logError << "write " << data.byteSize() <<"bytes to file = \"" << fullpath << "\"";
		return false;
	}

	boolean Storage::rewrite(ConstString path, uint64 offset, ConstBinaryData data)
	{
		String fullpath = root;
		fullpath << path;
		FILE* handle = fopen(fullpath.c_str(), "wb+");
		if(handle == 0){
			logError << "cant fopen " << path;
			return 0;
		}

		if(!fseek(handle, offset, SEEK_SET)){
			logError << "seek to " << offset <<" file = \"" << fullpath << "\"";
			return false;
		}

		uint64 writed = fwrite(data.first(), 1, data.byteSize(), handle);
		fclose(handle);
		if(writed == data.byteSize())
			return true;
		logError << "write " << data.byteSize() <<"bytes to file = \"" << fullpath << "\"";
		return false;
	}

	boolean Storage::erase(ConstString path)
	{
		String fullpath = root;
		fullpath << path;
		int ret = remove(fullpath.c_str());
		if(ret){
			logError << "Cant erase file " << fullpath;
			return false;
		}
		return true;
	}
}
