#include "Scene.h"


Scene::Scene():
	activeCameraID(0)
{

}


void Scene::addModel(std::shared_ptr<ModelObject> modelToAdd)
{
	_models.push_back(modelToAdd);
}

void Scene::addCamera(std::shared_ptr<Camera> cameraToAdd)
{
	_cameras.push_back(cameraToAdd);
}

std::vector<std::shared_ptr<ModelObject>> &Scene::getAllModels()
{
	return _models;
}

std::vector<std::shared_ptr<Camera>> &Scene::getAllCameras()
{
	return _cameras;
}

std::shared_ptr<Camera> &Scene::getActiveCamera()
{
	return _cameras[activeCameraID];
}
