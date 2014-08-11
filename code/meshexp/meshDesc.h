#include "include.h"
#include "../reflection/include.h"
#include "../drawer/include.h"
#include "../drawer/reflection.h"

namespace MeshExpLib
{
using namespace ReflectionLib;
using namespace DrawerLib;

class Mesh : public ReflectionLib::Object
{
public:
	rl_class(MeshExpLib::Mesh)
	rl_reflective
	rl_base_class0(ReflectionLib::Object)

	class IBoneTMGetter
	{
	public:
		virtual void getBoneTM(Matrix34f& tm, int boneId, Mesh& mesh)=0;
	};

	class Chunk : public ReflectionLib::Object
	{
	public:
		rl_class(MeshExpLib::Mesh::Chunk)
		rl_reflective
		rl_base_class0(ReflectionLib::Object)

		rl_field(String, materialName)
		rl_field(DrawerLib::EMaterialMethaType, materialMethaType)
		rl_field(boolean, metal)
		rl_field(uint, frame)
		rl_field(uint, frameCount)
		rl_field(uint, lod)
		rl_field(uint, id)
		rl_field(Vector2f, texCoordMult)
		rl_field(Vector2f, texCoordAdd)
		rl_field(DrawerLib::EVertexFormat, vertexFormat)
		rl_field(BinaryData, vertexData)
		rl_field(BinaryData, indexData)

		rl_field(Vector3f, aabbMin);
		rl_field(Vector3f, aabbMax);

		rl_field(String, bone)
		rl_field(int, boneId)

		Chunk()
		{
			materialMethaType = EMaterialMethaType_Solid;
			metal = false;
			vertexFormat = DrawerLib::EVertexFormat_None;
			texCoordMult.setOne();
			texCoordAdd.setZero();
			frame = 0;
			frameCount = 1;
			lod = 0;
			id = 0;
			aabbMin.setZero();
			aabbMax.setZero();
			boneId = -1;
		}

		void build(DrawerLib::IVertexBuffer& vb, uint& vbOffset, DrawerLib::IIndexBuffer& ib, uint& ibFirst)
		{
			_batchData.vb = &vb;
			_batchData.vbOffset = vbOffset;
			_batchData.vbSize = vertexData.byteSize();
			_batchData.vbFormat = vertexFormat;
			vb.copy(_batchData.vbOffset, vertexData.first(), _batchData.vbSize);
			vbOffset += _batchData.vbSize;
			vertexData.clear();

			_batchData.ib = &ib;
			_batchData.ibFirst = ibFirst;
			_batchData.trisCount = indexData.byteSize() / (sizeof(uint16) * 3);
			_batchData.ibStrip = false;
			ib.copy(_batchData.ibFirst, (uint16*)indexData.first(), indexData.byteSize() / sizeof(uint16));
			ibFirst += indexData.byteSize() / sizeof(uint16);
			indexData.clear();
		}

		const BatchBuffersDataEssential& batchData() { return _batchData; }

	private:
		BatchBuffersDataEssential _batchData;
	};

	class Bone
	{
	public:
		rl_class(MeshExpLib::Mesh::Bone)

		rl_field(String, name)
		rl_field(String, parent)
		rl_field(uint, id)
		rl_field(int, parentId)
		
		rl_field(Matrix34f, abs)
		rl_field(Matrix34f, absInv)
		rl_field(Matrix34f, loc)
		rl_field(Matrix34f, locInv)
		typedef Array<int16, SystemAllocatorWrapper, 32, true> ChildsArray;
		rl_field(ChildsArray, childs)

		Bone()
		{
			id = 0;
			parentId = -1;
			abs.setIdentity();
			absInv.setIdentity();
			loc.setIdentity();
			locInv.setIdentity();
		}
	};

	typedef ReflectionLib::HashArrayOfConstructables<Chunk, StrongPtr<Chunk> > ChunkArray;
	typedef Array<Bone> BoneArray;

	rl_field(ChunkArray, chunks)
	rl_field(BoneArray, bones)
	rl_field(uint, rootBonesCount)

	rl_field(Vector3f, aabbMin);
	rl_field(Vector3f, aabbMax);

	void build(DrawerLib::IDevice& dev)
	{
		vertexBuffer = dev.createVertexBuffer();
		indexBuffer = dev.createIndexBuffer();

		uint vsSize = 0;
		uint ibSize = 0;
		foreach(ich, chunks){
			vsSize += ich->val->vertexData.byteSize();
			ibSize += ich->val->indexData.byteSize();
		}

		vertexBuffer->init(vsSize);
		indexBuffer->init(ibSize / sizeof(uint16));

		uint vbOffset = 0;
		uint ibFirst = 0;
		foreach(ich, chunks)
			ich->val->build(*vertexBuffer, vbOffset, *indexBuffer, ibFirst);
	}

	DrawerLib::IDevice* device;
	rl_method_0(onLoadBegin)
	{
		chunks.clear();
		vertexBuffer = 0;
		indexBuffer = 0;
	}

	rl_method_0(onLoadEnd)
	{
		if(device)
			build(*device);
	}

	Mesh()
	{
		device = 0;
		aabbMin.setZero();
		aabbMax.setZero();
		rootBonesCount = 0;
	}

protected:
	StrongPtr<DrawerLib::IVertexBuffer> vertexBuffer;
	StrongPtr<DrawerLib::IIndexBuffer> indexBuffer;
};



}