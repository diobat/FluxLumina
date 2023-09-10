#pragma once

#include "scene\SceneObject.h"

class GraphicalEngine
{
public:
	GraphicalEngine();

	virtual void registerObject(SceneObject& object) = 0;
	virtual void unregisterObject(SceneObject& object) = 0;

};


