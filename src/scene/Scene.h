#pragma once

//	STD includes
#include <vector>

//	First Party includes
#include "Camera.h"
// #include "rendering/texture.h"
// #include "rendering/model.h"

#include "sceneObject.h"

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
	
	std::vector<SceneObject> _objects;

	std::vector<Camera> _cameras;
	std::vector<Model> _models;		// should be a member of SceneObject
	std::vector<Texture> _textures; // should be a member of SceneObject
};