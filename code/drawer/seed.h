#pragma once

#include "../base/include.h"
#include "../image/include.h"

namespace DrawerLib
{

USING_BASE_LIB
USING_BASE_MATH
using namespace ImageLib;

typedef Base::Vector2f Vector2;
typedef Base::Vector3f Vector2_t;
typedef Base::Vector4f Vector4;
typedef Base::Quaternionf Quaternion;
typedef Base::Matrix22f Matrix22;
typedef Base::Matrix23f Matrix23;
typedef Base::Matrix33f Matrix33;
typedef Base::Matrix34f Matrix34;
typedef Base::Matrix44f Matrix44;
//typedef Base::Matrix45f Matrix45;


class ITempDataBuffer
{
public:
	virtual byte* getTempVertexData(class IVertexBuffer*& outVertexBuffer, uint& outOffset, uint size)=0;
	virtual uint16* getTempIndexData(class IIndexBuffer*& outIndexBuffer, uint& outOffset, uint size)=0;
};

}
