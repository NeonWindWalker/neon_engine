#include "descs.h"
#include "meshDesc.h"
#include "../mander/include.h"

namespace MeshExpLib
{

class MeshOperator
{
public:
	Vector2f texMult;
	Vector2f texAdd;

	MeshOperator(const Vector2f& texMult, const Vector2f& texAdd)
	{
		this->texMult = texMult;
		this->texAdd = texAdd;
	}

	Vector3f getPosition(const MeshVertexFormat& v)
	{
		return v.position;
	}

	Vector2f getTexCoord(const MeshVertexFormat& v)
	{
		Vector2f t;
		DrawerLib::convert(t, v.texcoord);
		return t * texMult + texAdd;
	}

	void setTangent(MeshVertexFormat& v, const Vector3f& val)
	{
		DrawerLib::convert(v.tangent, val);
	}

	void setBinormal(MeshVertexFormat& v, const Vector3f& val)
	{
		DrawerLib::convert(v.binormal, val);
	}
};

extern boolean doExport(BinaryData& meshFile, BinaryData& collisionFile, ConstBinaryData src)
{
	RootArray rootArr;
	ReflectionLib::Reflection r = ReflectionLib::reflectionOf(rootArr);
	if(!r.deserialize(ConstString((const utf8*)src.first(), src.byteSize()))){
		logError << "cant parse source file";
		return false;
	}
	Mesh theMesh;

	foreach(it, rootArr)
	{
		Bone* srcBone = (Bone*)reflectionOf(**it).cast<Bone>().objectPtr();
		if(!srcBone)
			continue;

		Mesh::Bone* bone;
		if(srcBone->parent.empty())
			bone = theMesh.bones.insert(theMesh.rootBonesCount ++);
		else
			bone = &theMesh.bones.push();
		
		bone->name = srcBone->name;
		bone->parent = srcBone->parent;
		bone->id = srcBone->id;
		bone->abs = srcBone->matrix;
	}

	foreach(b, theMesh.bones)
	{
		foreach(b2, theMesh.bones)
		{
			if(b2->name == b->parent)
				b->parentId = theMesh.bones.indexOf(b2);
		}
	}

	foreach(b, theMesh.bones)
	{
		foreach(b2, theMesh.bones)
		{
			if(b2->parent == b->name)
				b->childs.push(theMesh.bones.indexOf(b2));
		}
	}

	Array<boolean> calculatedBones;
	foreach(b, theMesh.bones)
		calculatedBones.push(false);
	uint totCalcBonesCount = 0;
	while(totCalcBonesCount != theMesh.bones.size())
	{
		foreach(b, theMesh.bones)
		{
			if(calculatedBones[theMesh.bones.indexOf(b)])
				continue;

			boolean allParentCalculated = true;
			Mesh::Bone* p = b;
			while(p->parentId >= 0)
			{
				p = &theMesh.bones[p->parentId];
				if(!calculatedBones[theMesh.bones.indexOf(p)]){
					allParentCalculated = false;
					break;
				}
			}

			if(!allParentCalculated)
				continue;

			if(b->parentId >= 0)
			{
				Mesh::Bone* p = &theMesh.bones[b->parentId];
				b->loc = p->abs.invert() * b->abs;
			}
			else
			{
				b->loc = b->abs;
			}
			b->absInv = b->abs.invert();
			b->locInv = b->loc.invert();
			calculatedBones[theMesh.bones.indexOf(b)] = true;
			totCalcBonesCount ++;
			/*
			Matrix34 xxx = b->absInv * b->abs; 
			Matrix34 xxx2 = b->abs * b->absInv; 
			Matrix44 m4(b->abs, Vector4f(0,0,0,1));
			Matrix44 m4i = m4.invert();
			Matrix44 xx = m4i * m4; 
			int igsdaghe = 0;
			*/
		}
	}
	

	foreach(it, rootArr)
	{
		TriangleGeometry* tg = (TriangleGeometry*)reflectionOf(**it).cast<TriangleGeometry>().objectPtr();
		if(!tg)
			continue;

		foreach(vit, tg->vertexes)
		{
			vit->p = tg->matrix * vit->p;
			Matrix33 N;
			N.x = tg->matrix.x / tg->matrix.x.lenghtSquare();
			N.y = tg->matrix.y / tg->matrix.y.lenghtSquare();
			N.z = tg->matrix.z / tg->matrix.z.lenghtSquare();
			vit->n = (N * vit->n).normal();
		}

		if(tg->twoSided)
		{
			uint tris = tg->vertexes.size() / 3;
			tg->vertexes.resize(tris * 2 * 3);
			for(uint tr=0; tr < tris; ++tr)
			{
				tg->vertexes[(tris + tr) * 3 + 0] = tg->vertexes[tr * 3 + 2];
				tg->vertexes[(tris + tr) * 3 + 0].n = -tg->vertexes[tr * 3 + 2].n;
				tg->vertexes[(tris + tr) * 3 + 1] = tg->vertexes[tr * 3 + 1];
				tg->vertexes[(tris + tr) * 3 + 1].n = -tg->vertexes[tr * 3 + 1].n;
				tg->vertexes[(tris + tr) * 3 + 2] = tg->vertexes[tr * 3 + 0];
				tg->vertexes[(tris + tr) * 3 + 2].n = -tg->vertexes[tr * 3 + 0].n;
			}
		}
	}

	for(int i = rootArr.size() - 1; i >= 0; --i)
	{
		TriangleGeometry* tg = (TriangleGeometry*)reflectionOf(*rootArr[i]).cast<TriangleGeometry>().objectPtr();
		if(!tg)
			continue;

		for(int i2 = 0; i2 < i; i2++)
		{
			TriangleGeometry* tg2 = (TriangleGeometry*)reflectionOf(*rootArr[i2]).cast<TriangleGeometry>().objectPtr();
			if(!tg2)
				continue;

			if(tg->material == tg2->material && tg->parent == tg2->parent && !tg->isBone && !tg2->isBone && tg->materialMethaType == tg2->materialMethaType && tg->frame == tg2->frame && tg->id == tg2->id && tg->metal == tg2->metal)
			{
				tg2->vertexes.push(tg->vertexes);
				rootArr.erase(i);
				break;
			}
		}
	}

	
	foreach(it, rootArr)
	{
		ReflectionLib::IReflective* obj = *it;
		if(!obj)
			continue;
		TriangleGeometry* tg = (TriangleGeometry*)reflectionOf(*obj).cast<TriangleGeometry>().objectPtr();
		if(!tg)
			continue;
		if(tg->vertexes.size() < 3)
			continue;
		
		Mesh::Chunk* meshBase;
		Mesh::Chunk* mesh = (Mesh::Chunk*)theMesh.chunks.insertAndConstruct(meshBase, typeOf<Mesh::Chunk>(), tg->name);
		if(!mesh)
			continue;

		Vector2f tMin,tMax;
		tMin = tMax = tg->vertexes[0].t;
		mesh->aabbMin = mesh->aabbMax = tg->vertexes[0].p;
		foreach(vit, tg->vertexes)
		{
			tMin.selfMinimalize(vit->t);
			tMax.selfMaximalize(vit->t);
			mesh->aabbMin.selfMinimalize(vit->p);
			mesh->aabbMax.selfMaximalize(vit->p);
		}

		if(rootArr.indexOf(it) == 0)
		{
			theMesh.aabbMin = mesh->aabbMin;
			theMesh.aabbMax = mesh->aabbMax;
		}
		else
		{
			theMesh.aabbMin.selfMinimalize(mesh->aabbMin);
			theMesh.aabbMax.selfMaximalize(mesh->aabbMax);
		}

		Array<MeshVertexFormat> srcArr;
		foreach(vit, tg->vertexes)
		{
			MeshVertexFormat& d = srcArr.push();
			memoryClear(&d, sizeof(d));
			d.position = vit->p;
			DrawerLib::convert(d.texcoord, (vit->t - tMin) / (tMax - tMin) * 2.0f - Vector2f(1.0f, 1.0f));
			DrawerLib::convert(d.normal, vit->n);
			d.opacity = 255;
			d.unocclusion = 255;
		}
		mesh->texCoordMult = (tMax - tMin) * 0.5f;
		mesh->texCoordAdd = tMin + mesh->texCoordMult;

		String matNameFixed = tg->material;
		foreach(c, matNameFixed)
			if(*c == '\\')
				*c = '/';
		mesh->materialName = matNameFixed;

		uint trisCount = srcArr.size() / 3;
		MeshVertexFormat* aV;
		uint32* aI;
		uint vertexCount;
		if(!Mander::optimize(aV, vertexCount, aI, srcArr.first(), trisCount, 1))
			continue;

		Mander::calcTangentAndBinormal(MeshOperator(mesh->texCoordMult, mesh->texCoordAdd), aV, vertexCount, aI, trisCount);

		if(tg->isBone)
			mesh->bone = tg->name;
		else
			mesh->bone = tg->parent;

		foreach(b, theMesh.bones)
			if(b->name == mesh->bone){
				mesh->boneId = theMesh.bones.indexOf(b);
				break;
			}

		mesh->materialMethaType = tg->materialMethaType;
		mesh->metal = tg->metal;
		mesh->frame = tg->frame;
		mesh->frameCount = tg->frameCount;
		mesh->lod = tg->lod;
		mesh->id = tg->id;
		mesh->vertexFormat = EVertexFormat_Mesh;
		mesh->vertexData.set(aV, sizeof(MeshVertexFormat) * vertexCount);
		uint16* aI16 = new uint16[srcArr.size()];
		for(uint i=0; i < srcArr.size(); ++i)
			aI16[i] = aI[i];
		mesh->indexData.set(aI16, sizeof(uint16) * srcArr.size());
		delete aV;
		delete aI;
		delete aI16;
	}

	
	ReflectionLib::Reflection meshArrRefl = ReflectionLib::reflectionOf(theMesh);
	String str;
	meshArrRefl.serialize(str);
	meshFile.set(str.first(), str.size());
	return true;
}

extern boolean doExport(FileStorageLib::IStorage& destFS, FileStorageLib::IStorage& srcFS)
{
	BinaryData bindata;
	if(!srcFS.read(bindata, _CS("meshexp.par"))){
		logError << "cant read params file";
		return false;
	}
	String localFileName((utf8*)bindata.first(), bindata.byteSize());
	//srcFS.erase(_CS("meshexp.par"));

	BinaryData srcFile;
	if(!srcFS.read(srcFile, _CS("meshexp.r"))){
		logError << "cant read source file";
		return false;
	}
	//srcFS.erase(_CS("meshexp.r"));

	BinaryData meshFile;
	BinaryData collisionFile;
	if(!doExport(meshFile, collisionFile, srcFile))
		return false;

	if(meshFile.byteSize())
		destFS.write(String() << localFileName << _CS(".mesh"), meshFile);
	if(collisionFile.byteSize())
		destFS.write(String() << localFileName << _CS(".col"), collisionFile);
	return true;
}

}
