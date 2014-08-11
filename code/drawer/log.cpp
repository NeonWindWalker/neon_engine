#include "log.h"
#include "interfaceDrawHelper.h"

namespace DrawerLib
{
	
void DebugDraw::line(const SimplePrimitiveVertexFormat& a, const SimplePrimitiveVertexFormat& b)
{
	LockGuard lg(mutex);
	SimplePrimitiveVertexFormat vert[2] = {a, b};
	drawer.lines(makeConstRange(vert,2));
}

void DebugDraw::triangle(const SimplePrimitiveVertexFormat& a, const SimplePrimitiveVertexFormat& b, const SimplePrimitiveVertexFormat& c)
{
	LockGuard lg(mutex);
	SimplePrimitiveVertexFormat vert[3] = {a, b, c};
	drawer.triangles(makeConstRange(vert,3));
}
	
}


