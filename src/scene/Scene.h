#pragma once

//	STD includes
#include <vector>

//	First Party includes
#include "Camera.h"
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
	std::vector<Texture>& getAllTextures();
	std::vector<Camera>& getAllCameras();
	Camera& getActiveCamera();

private:

	unsigned int activeCameraID;
	std::vector<Camera> _cameras;
	std::vector<Model> _models;
	std::vector<Texture> _textures;
};