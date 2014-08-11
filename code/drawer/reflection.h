#pragma once

#include "include.h"
#include "../reflection/include.h"

#define ReflectionEnum_EnumStrings  { "None", "Mesh", "SkeletalMesh", "LightProbedMesh", "BuildinLightingMesh", "HeightMap", "Particle", "InterfaceQuad", "InterfaceFont", "SimplePrimitive" }
#define ReflectionEnum_EnumValues  { DrawerLib::EVertexFormat_None, DrawerLib::EVertexFormat_Mesh, DrawerLib::EVertexFormat_SkeletalMesh, DrawerLib::EVertexFormat_LightProbedMesh, DrawerLib::EVertexFormat_BuildinLightingMesh, DrawerLib::EVertexFormat_HeightMap, DrawerLib::EVertexFormat_Particle, DrawerLib::EVertexFormat_InterfaceQuad, DrawerLib::EVertexFormat_InterfaceFont, DrawerLib::EVertexFormat_SimplePrimitive }
	ReflectionEnum(DrawerLib::EVertexFormat, DrawerLib::EVertexFormat_EnumCount)
#undef ReflectionEnum_EnumStrings
#undef ReflectionEnum_EnumValues


#define ReflectionEnum_EnumStrings  { "None", "Solid", "AlphaTest", "Transparent" }
#define ReflectionEnum_EnumValues  { DrawerLib::EMaterialMethaType_None, DrawerLib::EMaterialMethaType_Solid, DrawerLib::EMaterialMethaType_AlphaTest, DrawerLib::EMaterialMethaType_Transparent }
		ReflectionEnum(DrawerLib::EMaterialMethaType, DrawerLib::EMaterialMethaType_EnumCount)
#undef ReflectionEnum_EnumStrings
#undef ReflectionEnum_EnumValues