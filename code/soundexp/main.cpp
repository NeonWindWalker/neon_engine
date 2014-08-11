#include "include.h"
#include "../base/include.h"
#include "../sound/include.h"

namespace SoundExpLib
{
USING_BASE_LIB
using namespace SoundLib;

class Task : public RefCountable
{
public:
	FileStorageLib::IStorage& destFS;
	FileStorageLib::IStorage& srcFS;
	String path;
	Thread* thread;

	Task(FileStorageLib::IStorage& in_destFS, FileStorageLib::IStorage& in_srcFS, ConstString in_path) : destFS(in_destFS), srcFS(in_srcFS)
	{
		path = in_path;
		thread = new Thread(makeDelegate(this, &Task::exportSound));
	}

	~Task()
	{
		delete thread;
	}

	void exportSound()
	{
		if(!path.endsWith(_CS(".wav")))
			return;

		if(path.startsWith(_CS("music/")) || path.startsWith(_CS("sound/packed/")))
		{
			BinaryData data;
			if(!srcFS.read(data, path))
				return;

			SoundLib::SoundSystem system;
			SoundWavData* wav = new SoundWavData(system, data);
			if(!wav->frames()){
				logError << "bad file = \"" << path;
				return;
			}

			BinaryData opus;
			if(!wav->toOpus(opus)){
				logError << "bad toOpus = \"" << path;
				return;
			}

			String destPath = path;
			if(path.startsWith(_CS("sound/packed"))){
				destPath = _CS("sound/");
				destPath << path.afterLeftTrim(_CS("sound/packed/").size());
			}
			destPath.resize(destPath.size() - 4);
			destPath << _CS(".opus");
			destFS.write(destPath, opus);

			delete wav;
		}
		else if (path.startsWith(_CS("sound/unpacked/")))
		{
			BinaryData data;
			if(!srcFS.read(data, path))
				return;

			String destPath = path;
			destPath = _CS("sound/");
			destPath << path.afterLeftTrim(_CS("sound/unpacked/").size());
			destFS.write(destPath, data);
		}
	}
};



void doExport(FileStorageLib::IStorage& destFS, FileStorageLib::IStorage& srcFS)
{
	Array<String> files;
	srcFS.list(files, _CS("sound"), _CS("sound"));
	srcFS.list(files, _CS("music"), _CS("music"));
	Array<StrongPtr<Task> > tasks;
	for(uint i=0; i < files.size(); ++i)
	{
		if(!files[i].endsWith(_CS(".wav")))
			continue;
		tasks.push( new Task(destFS, srcFS, files[i]));
	}
	tasks.clear();
}

}