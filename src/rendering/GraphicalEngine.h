#pragma once

// First-party includes
#include "rendering\mesh.h"
#include "rendering\texture.h"
#include "scene/Scene.h"

class GraphicalEngine
{
public:
	GraphicalEngine();
	
	virtual void initializeMesh(Mesh &mesh) = 0;
	virtual void initializeTexture(Texture &texture) = 0;

protected:
	std::shared_ptr<Scene> _scene;
};


