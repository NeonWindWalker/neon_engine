#include "include.h"
#include "../reflection/include.h"
#include "../drawer/include.h"
#include "../drawer/reflection.h"

namespace MeshExpLib
{
using namespace ReflectionLib;

typedef ReflectionLib::ArrayOfConstructables<ReflectionLib::Object, StrongPtr<ReflectionLib::Object> > RootArray;

class TriangleGeometry : public ReflectionLib::Object
{
public:
	rl_class(MeshExpLib::TriangleGeometry)
	rl_reflective
	rl_base_class0(ReflectionLib::Object)

	class Vertex
	{
	public:
		rl_class(MeshExpLib::TriangleGeometry::Vertex)
		
		rl_field(Vector3f, p)
		rl_field(Vector3f, n)
		rl_field(Vector2f, t)
		rl_field(Vector4f, bi)
		rl_field(Vector4f, bw)

		Vertex()
		{
			p.setZero();
			n.setZero();
			t.setZero();
			bi.setZero();
			bw.setZero();
		}
	};

	rl_field(boolean, isMesh)
	rl_field(boolean, isCollision)
	rl_field(boolean, isBone)
	rl_field(String, name)
	rl_field(String, material)
	rl_field(DrawerLib::EMaterialMethaType, materialMethaType)
	rl_field(boolean, metal)
	rl_field(uint, frame)
	rl_field(uint, frameCount)
	rl_field(uint, lod)
	rl_field(uint, id)
	rl_field(boolean, twoSided)
	rl_field(Matrix34f, matrix)
	rl_field(String, parent)
	rl_field(Array<Vertex>, vertexes)

	TriangleGeometry()
	{
		isMesh = true;
		isCollision = false;
		isBone = false;
		materialMethaType = DrawerLib::EMaterialMethaType_Solid;
		metal = false;
		frame = 0;
		frameCount = 1;
		lod = 0;
		id = 0;
		twoSided = false;
		matrix.setIdentity();
	}
};


class Bone : public ReflectionLib::Object
{
public:
	rl_class(MeshExpLib::Bone)
	rl_reflective
	rl_base_class0(ReflectionLib::Object)

	rl_field(String, name)
	rl_field(String, parent)
	rl_field(Matrix34f, matrix)
	rl_field(uint, id)

	Bone()
	{
		id = 0;
		matrix.setIdentity();
	}
};

}