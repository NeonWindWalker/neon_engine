#ifdef __ANDROID__

#include "myjni.h"
#include "../base/scalar.h"
#include "../base/log.h"
#include "include.h"

namespace FileStorageLib
{

class AssetStorage : public IStorage
{
private:
	//JNIEnv* jENV;
	JavaVM* jVM;
	jobject jAssetReader;
	jmethodID jSizeMethodId;
	jmethodID jExistsMethodId;
	jmethodID jReadMethodId;
	jmethodID jReadChunkMethodId;
	jmethodID jListMethodId;

public:
	AssetStorage(JavaVM* jVM, jobject jAssetReader)
	{
		this->jVM = jVM;
		this->jAssetReader = jAssetReader;

		JNIEnv* env = getThreadJavaEnviroment(jVM);

		jclass cls = env->GetObjectClass(jAssetReader);
		if(!cls)
			logError << "cant find AssetReader java class";

		jSizeMethodId = env->GetMethodID(cls, "size", "(Ljava/lang/String;)I");
		if(!jSizeMethodId)
			logError << "cant find size method in AssetReader java class";

		jExistsMethodId = env->GetMethodID(cls, "exists", "(Ljava/lang/String;)Z");
		if(!jExistsMethodId)
			logError << "cant find exists method in AssetReader java class";

		jReadMethodId = env->GetMethodID(cls, "read", "(Ljava/lang/String;)[B");
		if(!jReadMethodId)
			logError << "cant find read method in AssetReader java class";

		jReadChunkMethodId = env->GetMethodID(cls, "readChunk", "(Ljava/lang/String;II)[B");
		if(!jReadChunkMethodId)
			logError << "cant find readChunk method in AssetReader java class";

		jListMethodId = env->GetMethodID(cls, "list", "(Ljava/lang/String;Ljava/lang/String;)[Ljava/lang/String;");
		if(!jListMethodId)
			logError << "cant find list method in AssetReader java class";

		env->DeleteLocalRef(cls);
	}

	Base::boolean size(uint64& size, ConstString path)
	{
		JNIEnv* env = getThreadJavaEnviroment(jVM);

		jstring str = jStringFromString(env, path);
		jint s = env->CallIntMethod(jAssetReader, jSizeMethodId, str);
		env->DeleteLocalRef(str);
		if(s < 0){
			logError << "get size of file: " << path;
			return false;
		}
		size = s;
		return true;
	}

	Base::boolean exists(ConstString path)
	{
		JNIEnv* env = getThreadJavaEnviroment(jVM);

		jstring str = jStringFromString(env, path);
		jboolean ret = env->CallBooleanMethod(jAssetReader, jExistsMethodId, str);
		env->DeleteLocalRef(str);
		return ret;
	}

	Base::boolean read(BinaryData& data, ConstString path)
	{
		JNIEnv* env = getThreadJavaEnviroment(jVM);
		
		jstring pathstr = jStringFromString(env, path);
		jobject arr = env->CallObjectMethod(jAssetReader, jReadMethodId, pathstr);
		env->DeleteLocalRef(pathstr);
		if(!arr){
			logError << "read " << path;
			return false;
		}
		int size = env->GetArrayLength((jbyteArray)arr);
		data.resize(size);
		env->GetByteArrayRegion((jbyteArray)arr, 0, size, (jbyte*)data.first());
		env->DeleteLocalRef(arr);
		return true;
	}

	Base::boolean read(BinaryDataProxy& data, ConstString path, uint64 offset)
	{
		JNIEnv* env = getThreadJavaEnviroment(jVM);

		jstring pathstr = jStringFromString(env, path);
		jobject arr = env->CallObjectMethod(jAssetReader, jReadChunkMethodId, pathstr, (jint)offset, (jint)data.byteSize());
		env->DeleteLocalRef(pathstr);
		if(!arr){
			logError << "read " << path << " offset " << offset << " size " << data.byteSize();
			return false;
		}
		env->GetByteArrayRegion((jbyteArray)arr, 0, data.byteSize(), (jbyte*)data.first());
		env->DeleteLocalRef(arr);
		return true;
	}

	Base::boolean list(Array<String>& files, ConstString path, ConstString basepath)
	{
		JNIEnv* env = getThreadJavaEnviroment(jVM);

		jstring pathstr = jStringFromString(env, path);
		jstring basepathstr = jStringFromString(env, basepath);
		jobject res = env->CallObjectMethod(jAssetReader, jListMethodId, pathstr, basepathstr);
		env->DeleteLocalRef(pathstr);
		env->DeleteLocalRef(basepathstr);
		if(res == 0){
			logError << "list " << path;
			return false;
		}

		int size = env->GetArrayLength((jarray)res);
		for(int i=0; i < size; ++i)
		{
			jobject obj = env->GetObjectArrayElement((jobjectArray)res, i);
			*files.push() = stringFromJString(env, (jstring)obj);
			env->DeleteLocalRef(obj);
		}
		env->DeleteLocalRef(res);
		return true;
	}

	Base::boolean write(ConstString path, ConstBinaryData data)
	{
		logError << "write to assets";
		return false;
	}

	Base::boolean append(ConstString path, ConstBinaryData data)
	{
		logError << "write to assets";
		return false;
	}

	Base::boolean rewrite(ConstString path, uint64 offset, ConstBinaryData data)
	{
		logError << "write to assets";
		return false;
	}


	Base::boolean erase(ConstString path)
	{
		logError << "erase assets";
		return false;
	}
};

extern IStorage* openAssetStorage(void* jVM, void* jAssetReader)
{
	return new AssetStorage((JavaVM*)jVM, (jobject)jAssetReader);
}

}
#endif