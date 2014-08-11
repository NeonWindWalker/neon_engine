#pragma once

#include "vertexFormat.h"

namespace DrawerLib
{
	
class InterfaceDrawHelper;

class DebugDraw
{
public:
	InterfaceDrawHelper& drawer;
	DebugDraw(InterfaceDrawHelper& impl) : drawer(impl) {}
	
	void line(const SimplePrimitiveVertexFormat& a, const SimplePrimitiveVertexFormat& b);
	void triangle(const SimplePrimitiveVertexFormat& a, const SimplePrimitiveVertexFormat& b, const SimplePrimitiveVertexFormat& c);
	
private:
	Mutex mutex;
};

extern DebugDraw* gDebugDraw;

}
