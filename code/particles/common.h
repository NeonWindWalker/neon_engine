#include "scene.h"
#include "view.h"
#include "atlas.h"

namespace ParticlesLib
{


static INLINE void pushBillboad(const AnimatedImageInfo& aii, View& view, float& savedR, const Vector3f& center, const Vector3f& scale, float rotation = 0.0f, const Vector4f& color = Vector4f(1.0f,1.0f,1.0f,1.0f), const float hue = 0.0f, const float saturation = 1.0f, float progress = 0.0f)
{
	if(savedR < 0)
		savedR = view.place.p.distanceSquareTo(center);
	Quad* q = view.pushQuad(savedR);
	Vector2f cs;
	cosSin(cs.x, cs.y, rotation);
	Vector3f X = view.place.x * cs.x + view.place.y * cs.y;
	Vector3f Y = -view.place.x * cs.y + view.place.y * cs.x;
	X *= scale.x;
	Y *= scale.y;
	Vector3f c = center + (view.place.p - center).normalFast() * scale.z;
	q->v[0][0].position = c - X - Y;
	q->v[0][1].position = c + X - Y;
	q->v[1][0].position = c - X + Y;
	q->v[1][1].position = c + X + Y;
	q->v[0][0].color = color;
	q->v[0][1].color = color;
	q->v[1][0].color = color;
	q->v[1][1].color = color;
	q->v[0][0].depth = scale.z;
	q->v[0][1].depth = scale.z;
	q->v[1][0].depth = scale.z;
	q->v[1][1].depth = scale.z;
	q->v[0][0].hue = hue;
	q->v[0][1].hue = hue;
	q->v[1][0].hue = hue;
	q->v[1][1].hue = hue;
	uint8_01 sat = flooru8_01(saturation);
	q->v[0][0].saturation = sat;
	q->v[0][1].saturation = sat;
	q->v[1][0].saturation = sat;
	q->v[1][1].saturation = sat;
	q->setTexCoordsByProgress01(aii, progress);
}

static INLINE void pushBillboad(const AnimatedImageInfo* aii, View& view, float& savedR, const Vector3f& center, const Vector3f& scale, float rotation = 0.0f, const Vector4f& color = Vector4f(1.0f,1.0f,1.0f,1.0f), const float hue = 0.0f, const float saturation = 1.0f, float progress = 0.0f)
{
	if(aii)
		pushBillboad(*aii, view, savedR, center, scale, rotation, color, hue, saturation, progress);
}

static INLINE void pushBillboadHalfXDirectional(const AnimatedImageInfo& aii, View& view, float& savedR, const Vector3f& center, const Vector3f& direction, const Vector3f& scale, const Vector4f& color = Vector4f(1.0f,1.0f,1.0f,1.0f), const float hue = 0.0f, const float saturation = 1.0f, float progress = 0.0f)
{
	if(savedR < 0)
		savedR = view.place.p.distanceSquareTo(center);
	Quad* q = view.pushQuad(savedR);
	Vector3f X = direction;
	Vector3f Y = view.place.z.cross(direction);
	if(Y.lenghtSquare() < 1e-16f)
		return;
	Y.selfNormalize();
	X *= scale.x;
	Y *= scale.y;
	Vector3f c = center + (view.place.p - center).normalFast() * scale.z;
	q->v[0][0].position = c - Y;
	q->v[0][1].position = c + X - Y;
	q->v[1][0].position = c + Y;
	q->v[1][1].position = c + X + Y;
	q->v[0][0].color = color;
	q->v[0][1].color = color;
	q->v[1][0].color = color;
	q->v[1][1].color = color;
	q->v[0][0].depth = scale.z;
	q->v[0][1].depth = scale.z;
	q->v[1][0].depth = scale.z;
	q->v[1][1].depth = scale.z;
	q->v[0][0].hue = hue;
	q->v[0][1].hue = hue;
	q->v[1][0].hue = hue;
	q->v[1][1].hue = hue;
	uint8_01 sat = flooru8_01(saturation);
	q->v[0][0].saturation = sat;
	q->v[0][1].saturation = sat;
	q->v[1][0].saturation = sat;
	q->v[1][1].saturation = sat;
	q->setTexCoordsByProgress01(aii, progress);
}

static INLINE void pushBillboadHalfXDirectional(const AnimatedImageInfo* aii, View& view, float& savedR, const Vector3f& center, const Vector3f& direction, const Vector3f& scale, const Vector4f& color = Vector4f(1.0f,1.0f,1.0f,1.0f), const float hue = 0.0f, const float saturation = 1.0f, float progress = 0.0f)
{
	if(aii)
		pushBillboadHalfXDirectional(*aii, view, savedR, center, direction, scale, color, hue, saturation, progress);
}


static INLINE float arrowFade(View& view, const Vector3f& center, const Vector3f& rayDirNormalized, float fadeRadius)
{
	Vector3f d = view.place.p - center;
	return min(d.subdot(rayDirNormalized).lenght() / fadeRadius, 1.0f);
}

static INLINE void pushBillboadHalfX(const AnimatedImageInfo& aii, View& view, float& savedR, const Vector3f& center, const Vector3f& X, const Vector3f& Y, float depth, const Vector4f& color, const float hue, const float saturation, float progress = 0)
{
	if(savedR < 0)
		savedR = view.place.p.distanceSquareTo(center);
	Quad* q = view.pushQuad(savedR);
	Vector3f c = center + (view.place.p - center).normalFast() * depth;
	q->v[0][0].position = c - Y;
	q->v[0][1].position = c + X - Y;
	q->v[1][0].position = c + Y;
	q->v[1][1].position = c + X + Y;
	q->v[0][0].color = color;
	q->v[0][1].color = color;
	q->v[1][0].color = color;
	q->v[1][1].color = color;
	q->v[0][0].depth = depth;
	q->v[0][1].depth = depth;
	q->v[1][0].depth = depth;
	q->v[1][1].depth = depth;
	q->v[0][0].hue = hue;
	q->v[0][1].hue = hue;
	q->v[1][0].hue = hue;
	q->v[1][1].hue = hue;
	uint8_01 sat = flooru8_01(saturation);
	q->v[0][0].saturation = sat;
	q->v[0][1].saturation = sat;
	q->v[1][0].saturation = sat;
	q->v[1][1].saturation = sat;
	q->setTexCoordsByProgress01(aii, progress);
}

static INLINE void pushBillboadHalfX(const AnimatedImageInfo* aii, View& view, float& savedR, const Vector3f& center, const Vector3f& X, const Vector3f& Y, float depth, const Vector4f& color, const float hue, const float saturation, float progress = 0)
{
	if(aii)
		pushBillboadHalfX(*aii, view, savedR, center, X, Y, depth, color, hue, saturation, progress);
}

/*
class SimpleSingleParticleDesc
{
public:
	rl_class(SimpleSingleParticleDesc)

	rl_field(String, image)
	rl_field(float, periodTime)
	rl_field(Vector3f, size)

	const AnimatedImageInfo* imageInfo;

	SimpleSingleParticleDesc()
	{
		periodTime = 1;
		size.setOne();
		imageInfo = 0;
	}
};

class SimpleSingleParticle : public Source
{
public:
	rl_class(Vector3f, size)

	float progress;

	virtual void update(Scene& scene, float dt)
	{
		progress += dt;
		progress = pfrac(progress);
	}

	virtual void visualize(Scene& scene, View& view, Atlas& atlas)
	{

	}
};
*/

}