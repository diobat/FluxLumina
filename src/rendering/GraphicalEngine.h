#pragma once

// First-party includes
#include "scene\SceneObject.h"
#include "rendering\mesh.h"
#include "rendering\texture.h"

class GraphicalEngine
{
public:
	GraphicalEngine();
	
	virtual void initializeMesh(Mesh &mesh) = 0;
	virtual void initializeTexture(Texture &texture) = 0;
};


