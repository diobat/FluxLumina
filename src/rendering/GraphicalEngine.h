#pragma once

#include "scene\SceneObject.h"
#include "rendering\mesh.h"

class GraphicalEngine
{
public:
	GraphicalEngine();
	
	virtual void initializeMesh(Mesh &mesh) = 0;
};


