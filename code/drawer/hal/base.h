#pragma once

#include "../include.h"
#include "../platformspecific.h"

#if !defined(DRAWER_API_OPENGL) && !defined(DRAWER_API_OPENGL_ES2)

	#if defined(__linux__) && !defined(__ANDROID__)
		#define DRAWER_API_OPENGL
	#endif

	#if defined(__ANDROID__)
		#define DRAWER_API_OPENGL_ES2
	#endif

    #if defined(__APPLE__)
        #define DRAWER_API_OPENGL_ES2
    #endif

	#if defined(_WIN32)
		#define DRAWER_API_OPENGL
		//#define DRAWER_API_DIRECT3D9
	#endif

#endif

namespace DrawerLib
{

static INLINE void buildPerspectiveProjectionMatrix(Matrix44& proj, float tanHor, float tanVer, float _near, float _far)
{
	proj.x.x = 1.0f / tanHor;
	proj.y.x = 0;
	proj.z.x = 0;
	proj.w.x = 0;

	proj.x.y = 0;
	proj.y.y = 1.0f / tanVer;
	proj.z.y = 0;
	proj.w.y = 0;

	proj.x.z = 0;
	proj.y.z = 0;
	proj.z.z = - _far / (_far - _near);
	proj.w.z = proj.z.z * _near;

	proj.x.w = 0;
	proj.y.w = 0;
	proj.z.w = -1.0f;
	proj.w.w = 0;
}
	
static INLINE void buildPerspectiveProjectionMatrix(Matrix44& proj, float tanHor, float tanVer, float _near)
{
	proj.x.x = 1.0f / tanHor;
	proj.y.x = 0;
	proj.z.x = 0;
	proj.w.x = 0;

	proj.x.y = 0;
	proj.y.y = 1.0f / tanVer;
	proj.z.y = 0;
	proj.w.y = 0;

	proj.x.z = 0;
	proj.y.z = 0;
	proj.z.z = -1.0f;
	proj.w.z = -_near;

	proj.x.w = 0;
	proj.y.w = 0;
	proj.z.w = -1.0f;
	proj.w.w = 0;
}

static INLINE void buildPerspectiveProjectionHackedForLinearizingZMatrix(Matrix44& proj, float tanHor, float tanVer, float _far)
{
	proj.x.x = 1.0f / tanHor;
	proj.y.x = 0.0f;
	proj.z.x = 0.0f;
	proj.w.x = 0.0f;

	proj.x.y = 0.0f;
	proj.y.y = 1.0f / tanVer;
	proj.z.y = 0.0f;
	proj.w.y = 0.0f;

	proj.x.z = 0.0f;
	proj.y.z = 0.0f;
	proj.z.z = -1.0f / _far;
	proj.w.z = 0.0f;

	proj.x.w = 0.0f;
	proj.y.w = 0.0f;
	proj.z.w = -1.0f;
	proj.w.w = 0.0f;
}

static INLINE void buildOrthoProjectionMatrix(Matrix44& proj, float halfWidth, float halfHeight, float _near, float _far)
{
	proj.x.x = 1.0f / halfWidth;
	proj.y.x = 0.0f;
	proj.z.x = 0.0f;
	proj.w.x = 0.0f;

	proj.x.y = 0.0f;
	proj.y.y = 1.0f / halfHeight;
	proj.z.y = 0.0f;
	proj.w.y = 0.0f;

	proj.x.z = 0.0f;
	proj.y.z = 0.0f;
	proj.z.z = - 1.0f / (_far - _near);
	proj.w.z = - _near * proj.z.z;

	proj.x.w = 0.0f;
	proj.y.w = 0.0f;
	proj.z.w = 0.0f;
	proj.w.w = 1.0f;
}

}

