#ifdef __linux__
#include "include.h"
#include "stdio.h"
#include "../base/scalar.h"
#include "../base/log.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

namespace FileStorageLib
{

Storage::Storage(ConstString path)
{
	if(!path.empty()){
		root = path;
	}else{
		char path[4096];
		getcwd(path, 4096);
		root = constString(path);
	}
	
	if(!root.empty() && !root.last() != '/')
		root << _CS("/");
}

boolean Storage::list(Array<String>& files, ConstString path, ConstString basepath)
{
	DIR *dp = opendir((String() << root << path).c_str());
	if(!dp)
		return false;

	dirent *dirp;
	while ((dirp = readdir(dp)) != 0){
		String s = constString(dirp->d_name);
		if(s == _CS("..") || s == _CS(".") || s.startsWith(_CS(".")) )
			continue;

		String basename;
		if(!basepath.empty())
			basename << basepath << _CS("/") << s;
		else
			basename = s;

		if(dirp->d_type != DT_DIR)
		{//file
			files.push(basename);
		}
		else
		{//dir
			list(files, String() << path << _CS("/") << s, basename);
		}
	}
	closedir(dp);
	return true;
}

Base::boolean createDirectory(Base::ConstString path)
{
	mkdir(Base::String(path).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	return true;
}

boolean Storage::createDirectoryFromLocalFilePath(ConstString path)
{
	for(const utf8* c = path.first(); c < path.end(); ++c)
	{
		if(*c == '/' || *c == '\\')
		{
			if(!createDirectory(String() << root << ConstString(path.first(), c)))
				return false;
			while(c != path.end() && (*c == '/' || *c == '\\'))
				++c;
		}
	}
	return true;
}

IStorage* openStorage(ConstString rootPath)
{
	return new Storage( rootPath );
}

}

#endif
