#include "Scene.h"


Scene::Scene():
	activeCameraID(0)
{

}

int Scene::initialize()
{
	// Import Model
	Model model("res/models/alliance.obj");
	addModel(std::move(model));

	// Import texture
	Texture texture;
	texture.load("res/models/alliance.png");
	texture.bind();

	// Add camera
	_cameras.push_back(Camera());
	return 1;
}


void Scene::addModel(const Model& modelToAdd)
{
	_models.push_back(modelToAdd);
}

void Scene::removeMesh()
{

}

std::vector<Model>& Scene::getAllModels()
{
	return _models;
}

std::vector<Texture>& Scene::getAllTextures()
{
	return _textures;
}

std::vector<Camera>& Scene::getAllCameras()
{
	return _cameras;
}

Camera& Scene::getActiveCamera()
{
	return _cameras[activeCameraID];
}
