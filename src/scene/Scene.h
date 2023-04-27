#pragma once

//	STD includes
#include <vector>

//	First Party includes
#include "Camera.h"

//

#include "rendering/Texture.h"
#include "rendering/Model.h"


class Scene
{
public:

	Scene();

	int initialize();
	void addModel(const Model& modelToAdd);
	void removeMesh();

	std::vector<Model>& getAllModels();

private:


	std::vector<Camera> _cameras;
	std::vector<Model> _models;

};