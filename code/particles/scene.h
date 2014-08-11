#pragma once

#include "view.h"

namespace ParticlesLib
{

class Scene
{
public:
	Scene(boolean separateThread = false);

	ListRoot<View> views;
	ListRoot<Source> sources;

	void simmulateBegin(float dt, Atlas& atlas);
	void simmulateEnd();
};

}