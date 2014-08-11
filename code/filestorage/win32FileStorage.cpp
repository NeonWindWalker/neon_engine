#ifdef _WIN32

#include "include.h"
#include "../base/include.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <Shlobj.h>
#include <Shellapi.h>

namespace FileStorageLib
{

class Storage : public IStorage
{
private:
	String root;

public:

	Storage(const ConstString& path)
	{
		watchDirHandle = INVALID_HANDLE_VALUE;
		watchDirHandle2 = INVALID_HANDLE_VALUE;
		memoryClear(&watchDirOverlapped, sizeof(watchDirOverlapped));
		memoryClear(&watchFileOverlapped, sizeof(watchFileOverlapped));
		watchingThread = 0;

		if(path.find(_CS(":\\")) != path.end() || path.find(_CS("://")) != path.end()) //abs path
		{
			root = path;
		}
		else
		{
			uint masize = 4096;
			root.reserve(masize);
			root.forceSetSize(GetCurrentDirectoryA(masize, (char*)root.first()));
			if(*root.last() != '\\')
				root << _CS("\\");
			root << path;
			root.mayrealloc();
		}

		if(!root.empty() && *root.last() != '\\')
			root << _CS("\\");
	}

	~Storage()
	{
		watching(false);
	}

	virtual Base::boolean size(uint64& size, ConstString path)
	{
		String fullname = root;
		fullname << path;
		HANDLE handle = CreateFileA(fullname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(handle == INVALID_HANDLE_VALUE){
			logError << "cant open file:" << fullname;
			return false;
		}

		union{
			DWORD n[2];
			uint64 s;
		};
		s = 0;
		n[0] = GetFileSize(handle, &n[1]);
		size = s;
		CloseHandle(handle);
		return true;
	}

	virtual Base::boolean exists(ConstString path)
	{
		String fullname = root;
		fullname << path;
		HANDLE handle = CreateFileA(fullname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(handle == INVALID_HANDLE_VALUE)
			return false;

		CloseHandle(handle);
		return true;
	}

	virtual Base::boolean read(BinaryData& data, ConstString path)
	{
		String fullname = root;
		fullname << path;
		HANDLE handle = CreateFileA(fullname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(handle == INVALID_HANDLE_VALUE){
			logError << "cant open file:" << fullname;
			return false;
		}

		union{
			DWORD n[2];
			uint64 s;
		};
		s = 0;
		n[0] = GetFileSize(handle, &n[1]);

		if (s >= (1<<31)){
			logError << "very big file size for reading into memory";
			CloseHandle(handle);
			return false;
		}
		
		data.resize((uinteger)s);

		DWORD readed = 0;
		ReadFile(handle, data.first(), (DWORD)data.byteSize(), &readed, 0);
		CloseHandle(handle);
		if(readed == data.byteSize())
			return true;
		logError << "read " << s <<"bytes from file = \"" << fullname << "\"";
		return false;
	}

	virtual Base::boolean read(BinaryDataProxy& data, ConstString path, uint64 offset)
	{
		String fullname = root;
		fullname << path;
		HANDLE handle = CreateFileA(fullname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(handle == INVALID_HANDLE_VALUE){
			logError << "cant open file:" << fullname;
			return false;
		}

		LONG hiseek = (LONG)(offset >> 32);
		LONG lowseek = SetFilePointer(handle, (LONG)offset, &hiseek, FILE_BEGIN);
		if(offset != ((uint64(hiseek)<<32) | lowseek)){
			logError << "seek to" << offset <<" file = \"" << fullname << "\"";
		}

		DWORD readed = 0;
		ReadFile(handle, data.first(), (DWORD)data.byteSize(), &readed, 0);
		CloseHandle(handle);
		if(readed == data.byteSize())
			return true;
		logError << "read " << data.byteSize() <<"bytes from file = \"" << fullname << "\"";
		return false;
	}

	virtual Base::boolean write(ConstString path, ConstBinaryData data)
	{
		String fullname = root;
		fullname << path;
		createDirectoryFromLocalFilePath(path);
		HANDLE handle = CreateFileA(fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(handle == INVALID_HANDLE_VALUE){
			logError << "cant open file:" << fullname;
			return false;
		}

		DWORD writed = 0;
		WriteFile(handle, data.first(), (DWORD)data.byteSize(), &writed, 0);
		CloseHandle(handle);

		if(writed == data.byteSize())
			return true;
		logError << "write " << data.byteSize() <<"bytes to file = \"" << fullname << "\"";
		return false;
	}

	virtual Base::boolean append(ConstString path, ConstBinaryData data)
	{
		String fullname = root;
		fullname << path;
		createDirectoryFromLocalFilePath(path);
		HANDLE handle = CreateFileA(fullname.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(handle == INVALID_HANDLE_VALUE){
			logError << "cant open file:" << fullname;
			return 0;
		}

		SetFilePointer(handle, 0, 0, FILE_END);

		DWORD writed = 0;
		WriteFile(handle, data.first(), (DWORD)data.byteSize(), &writed, 0);
		CloseHandle(handle);
		if(writed == data.byteSize())
			return true;
		logError << "write " << data.byteSize() <<"bytes to file = \"" << fullname << "\"";
		return false;
	}

	virtual Base::boolean rewrite(ConstString path, uint64 offset, ConstBinaryData data)
	{
		String fullname = root;
		fullname << path;
		createDirectoryFromLocalFilePath(path);
		HANDLE handle = CreateFileA(fullname.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(handle == INVALID_HANDLE_VALUE){
			logError << "cant open file:" << fullname;
			return 0;
		}

		LONG lowseek = (LONG)offset;
		LONG hiseek = (LONG)(offset >> 32);
		SetFilePointer(handle, lowseek, &hiseek, FILE_BEGIN);

		DWORD writed = 0;
		WriteFile(handle, data.first(), (DWORD)data.byteSize(), &writed, 0);
		CloseHandle(handle);
		if(writed == data.byteSize())
			return true;
		logError << "write " << data.byteSize() <<"bytes to file = \"" << fullname << "\"";
		return false;
	}

	virtual Base::boolean erase(ConstString path)
	{
		String fullname = root;
		fullname << path;
		if(DeleteFileA(fullname.c_str()))
			return true;
		logError << "erase file = \"" << fullname << "\"";
		return false;
	}

	virtual Base::boolean list(Array<String>& files, ConstString inpath, ConstString basepath)
	{
		WIN32_FIND_DATAA ffd;

		String path = inpath;
		foreach(it, path)
			if(*it == '/')
				*it = '\\';

		HANDLE hFind = FindFirstFileA((String() << root << path << _CS("\\*")).c_str(), &ffd);

		if(hFind == INVALID_HANDLE_VALUE){
			logError << "Cant list directory:" << root << path << _CS("\\*");
			return false;
		}

		do
		{
			String name(constString(ffd.cFileName));
			if(name == _CS(".") || name == _CS(".."))
				continue;

			String basename;
			if(!basepath.empty())
				basename << basepath << _CS("/") << name;
			else
				basename = name;

			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && !(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				list(files, String() << path << _CS("\\") << name, basename);
			}
			else if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) && !(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				files.push(basename);
			}
		}
		while (FindNextFileA(hFind, &ffd) != 0);
		FindClose(hFind);
		return true;
	}

	Base::boolean createDirectoryFromLocalFilePath(ConstString path)
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

	Base::boolean createDirectory(const ConstString& path)
	{
		int ret = SHCreateDirectoryExA(0, String(path).c_str(), 0);
		if(ret != ERROR_SUCCESS && ret != ERROR_FILE_EXISTS && ret != ERROR_ALREADY_EXISTS){
			logError << "Cant create directory:" << path;
			return false;
		}
		else
			return true;
	}

	void fixToStdPath(String& path)
	{
		foreach(it, path)
			if(*it == '\\')
				*it = '/';
		for(uint i=0; i + 1 < path.size(); ++i)
			if(path[i] == '/' && path[i + 1] == '/')
				path.ferase(i);
	}

	Base::boolean localPathFromGlobal(String loc, String globPath)
	{
		fixToStdPath(globPath);
		String stdRoot = root;
		fixToStdPath(stdRoot);
		if(stdRoot.size() > globPath.size())
			return false;
		if(stdRoot != ConstString(globPath.first(), stdRoot.size()))
			return false;

		loc = globPath.afterLeftTrim(stdRoot.size());
		return true;
	}


	HANDLE watchDirHandle;
	HANDLE watchDirHandle2;
	OVERLAPPED watchDirOverlapped;
	OVERLAPPED watchFileOverlapped;
	Thread* watchingThread;
	volatile Base::boolean watchThreadStop;
	Mutex changeEventsListMutex;
	Array<FileChangedNotification> changeEventsList;

	uint ladleOutChangeEvents(Array<FileChangedNotification>& list)
	{
		LockGuard lg(changeEventsListMutex);
		uint s = list.size();

		for(uint existMode = 0; existMode < 2; ++ existMode)
			for(uint n = 0; n < changeEventsList.size(); ++n)
			{
				if(changeEventsList[n].exist != existMode)
					continue;

				Base::boolean exist = existMode;
				for(uint i = n + 1; i < changeEventsList.size(); ++i)
					if(changeEventsList[i].name == changeEventsList[n].name)
						exist = changeEventsList[i].exist;

				if(exist == existMode){
					Base::boolean bFindSame = false; 
					for(uint j = s; j < list.size(); ++j)
						if(list[j].name == changeEventsList[n].name && list[j].exist == changeEventsList[n].exist)
						{
							bFindSame = true;
							break;
						}
					if(bFindSame)
						continue;
					list.push(changeEventsList[n]);
				}
			}
	
		changeEventsList.clear();
		return list.size() - s;
	}

	Base::boolean watching(Base::boolean on)
	{
		if(on)
		{
			if(watchDirHandle != INVALID_HANDLE_VALUE)
				return true;

			WCHAR RootFolder[4098] = {0};
			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)root.first(), root.size(), RootFolder, 4096);

			watchDirHandle = CreateFileW(RootFolder,
				GENERIC_READ,
				FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
				NULL
				);

			watchDirHandle2 = CreateFileW(RootFolder,
				GENERIC_READ,
				FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
				NULL
				);

			if(watchDirHandle == INVALID_HANDLE_VALUE){
				logError << "Getting directory " << root << " handle";
				return false;
			}

			watchDirOverlapped.hEvent = CreateEvent(0, FALSE, FALSE, 0);
			watchFileOverlapped.hEvent = CreateEvent(0, FALSE, FALSE, 0);
			watchThreadStop = false;
			watchingThread = new Thread(makeDelegate(this, &Storage::watchThreadFunc));
			return true;
		}
		else
		{
			if(watchDirHandle == INVALID_HANDLE_VALUE)
				return true;

			watchThreadStop = true;
			delete watchingThread;
			watchingThread = 0;
			CloseHandle(watchDirOverlapped.hEvent);
			watchDirOverlapped.hEvent = INVALID_HANDLE_VALUE;
			CloseHandle(watchFileOverlapped.hEvent);
			watchFileOverlapped.hEvent = INVALID_HANDLE_VALUE;
			CloseHandle(watchDirHandle);
			watchDirHandle = INVALID_HANDLE_VALUE;
			CloseHandle(watchDirHandle2);
			watchDirHandle = INVALID_HANDLE_VALUE;
			return true;
		}
	}

	Base::boolean watching()
	{
		return watchDirHandle != INVALID_HANDLE_VALUE;
	}

	void watchThreadFunc()
	{
		DWORD nBufferLength = 32*1024;
		BYTE* lpDirEventBuffer = new BYTE[nBufferLength];
		BYTE* lpFileEventBuffer = new BYTE[nBufferLength];
		
		DWORD returnedDirBytes = 0;
		ReadDirectoryChangesW(watchDirHandle, lpDirEventBuffer, nBufferLength, TRUE,
			FILE_NOTIFY_CHANGE_DIR_NAME,
			&returnedDirBytes, &watchDirOverlapped, 0);

		DWORD returnedFileBytes = 0;
		ReadDirectoryChangesW(watchDirHandle2, lpFileEventBuffer, nBufferLength, TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
			&returnedFileBytes, &watchFileOverlapped, 0);

		HANDLE handles[2];
		handles[0] = watchDirOverlapped.hEvent;
		handles[1] = watchFileOverlapped.hEvent;

		while(!watchThreadStop)
		{
			DWORD dwWaitStatus = WaitForMultipleObjects(2, handles, false, 500);
			
			if(dwWaitStatus == WAIT_OBJECT_0)
			{
				DWORD seek = 0;
				PFILE_NOTIFY_INFORMATION pPrevOldNameNotify = 0;
				while(seek < nBufferLength)
				{
					PFILE_NOTIFY_INFORMATION pNotify = PFILE_NOTIFY_INFORMATION(lpDirEventBuffer + seek);
					seek += pNotify->NextEntryOffset;

					String dirPath;
					dirPath.reserve(8096);
					dirPath.forceSetSize(WideCharToMultiByte(CP_UTF8, 0, pNotify->FileName, pNotify->FileNameLength / 2, (LPSTR)dirPath.first(), 8096, 0, 0));

					if(pNotify->Action == FILE_ACTION_REMOVED)
					{
						Array<String> files;
						String dirStdPath = dirPath;
						fixToStdPath(dirStdPath);
						list(files, dirPath, dirStdPath);
						
						changeEventsListMutex.lock();

						foreach(it, files)
						{
							FileChangedNotification ev;
							ev.name = *it;
							ev.exist = false;
							changeEventsList.push(ev);
						}

						changeEventsListMutex.unlock();
						pPrevOldNameNotify = 0;
					}
					else if(pNotify->Action == FILE_ACTION_RENAMED_OLD_NAME)
					{
						pPrevOldNameNotify = pNotify;
					}
					else if(pNotify->Action == FILE_ACTION_RENAMED_NEW_NAME)
					{
						Array<String> files;
						list(files, dirPath, ConstString());

						changeEventsListMutex.lock();

						if(pPrevOldNameNotify)
						{
							String oldStdPath;
							oldStdPath.reserve(8096);
							oldStdPath.forceSetSize(WideCharToMultiByte(CP_UTF8, 0, pPrevOldNameNotify->FileName, pPrevOldNameNotify->FileNameLength / 2, (LPSTR)oldStdPath.first(), 8096, 0, 0));
							fixToStdPath(oldStdPath);
							if(!oldStdPath.empty())
								oldStdPath << _CS("/");

							foreach(it, files)
							{
								FileChangedNotification ev;
								ev.name = oldStdPath;
								ev.name << *it;
								ev.exist = false;
								changeEventsList.push(ev);
							}
						}
						
						String newStdPath = dirPath;
						fixToStdPath(newStdPath);
						if(!newStdPath.empty())
							newStdPath << _CS("/");
						foreach(it, files)
						{
							FileChangedNotification ev;
							ev.name = newStdPath;
							ev.name << *it;
							ev.exist = true;
							changeEventsList.push(ev);
						}

						changeEventsListMutex.unlock();
						pPrevOldNameNotify = 0;
					}
					else
					{
						pPrevOldNameNotify = 0;
					}
					
					if(pNotify->NextEntryOffset == 0)
						break;
				}

				ReadDirectoryChangesW(watchDirHandle, lpDirEventBuffer, nBufferLength, TRUE,
					FILE_NOTIFY_CHANGE_DIR_NAME,
					&returnedDirBytes, &watchDirOverlapped, 0);
			}
			else if(dwWaitStatus == WAIT_OBJECT_0 + 1)
			{
				DWORD seek = 0;
				while(seek < nBufferLength)
				{
					PFILE_NOTIFY_INFORMATION pNotify = PFILE_NOTIFY_INFORMATION(lpFileEventBuffer + seek);
					seek += pNotify->NextEntryOffset;

					FileChangedNotification ev;
					ev.name.reserve(8096);
					ev.name.forceSetSize(WideCharToMultiByte(CP_UTF8, 0, pNotify->FileName, pNotify->FileNameLength / 2, (LPSTR)ev.name.first(), 8096, 0, 0));
					fixToStdPath(ev.name);
					ev.exist = pNotify->Action != FILE_ACTION_REMOVED && pNotify->Action != FILE_ACTION_RENAMED_OLD_NAME;
					
					changeEventsListMutex.lock();
					changeEventsList.push(ev);
					changeEventsListMutex.unlock();

					if(pNotify->NextEntryOffset == 0)
						break;
				}

				ReadDirectoryChangesW(watchDirHandle2, lpFileEventBuffer, nBufferLength, TRUE,
					FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
					&returnedFileBytes, &watchFileOverlapped, 0);
			}
		}
		delete lpDirEventBuffer;
		delete lpFileEventBuffer;
	}

	Base::boolean execute(ConstString workdir, ConstString path, ConstString params, Base::boolean wait)
	{
		String fullWorkDir;
		fullWorkDir << root <<  workdir;
		String fullPath;
		fullPath << root <<  path;
		WCHAR Path[4098] = {0};
		WCHAR Workdir[4098] = {0};
		WCHAR Params[4098] = {0};
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)fullWorkDir.first(), fullWorkDir.size(), Workdir, 4096);
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)fullPath.first(), fullPath.size(), Path, 4096);
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)params.first(), params.size(), Params, 4096);

		SHELLEXECUTEINFOW lpExecInfo;
		lpExecInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
		lpExecInfo.lpFile = Path; // name of file that you want to execute/ print/ or open/ in your case Adobe Acrobat.
		lpExecInfo.fMask = SEE_MASK_DOENVSUBST | SEE_MASK_NOCLOSEPROCESS ;     
		lpExecInfo.hwnd = NULL;  
		lpExecInfo.lpVerb = L"open"; // to open  program
		lpExecInfo.lpParameters = Params; //  file name as an argument
		lpExecInfo.lpDirectory = Workdir;   
		lpExecInfo.nShow = SW_HIDE ;  // show command prompt with normal window size 
		lpExecInfo.hInstApp = (HINSTANCE) SE_ERR_DDEFAIL ;   //WINSHELLAPI BOOL WINAPI result;
		if(!ShellExecuteExW(&lpExecInfo))
			return false;

		if(wait)
		{
			//wait until a file is finished printing
			if(lpExecInfo.hProcess != NULL)
			{
				::WaitForSingleObject(lpExecInfo.hProcess, INFINITE);
				::CloseHandle(lpExecInfo.hProcess);
			}
		}

		return true;
	}
};

IStorage* openStorage(ConstString rootPath)
{
	return new Storage( rootPath );
}

}

#endif