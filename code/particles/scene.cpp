#include "scene.h"

namespace ParticlesLib
{

Scene::Scene(boolean separateThread)
{

}

void Scene::simmulateBegin(float dt, Atlas& atlas)
{
	foreach(s, sources)
		s->update(*this, dt);

	foreach(v, views)
	{
		v->callBeginImpl();
		foreach(s, sources)
			s->visualize(*this, *v, atlas);
		v->callEndImpl();
	}
}

}

