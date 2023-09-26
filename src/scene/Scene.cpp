#include "Scene.h"


Scene::Scene():
	activeCameraID(0),
	_ambientLight(0.1f, { 1.0f, 1.0f, 1.0f }),
	_diffuseLight(0.15f, { 1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f })
{
	;
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

AmbientLight &Scene::getAmbientLight()
{
	return _ambientLight;
}

DiffuseLight &Scene::getDiffuseLight()
{
	return _diffuseLight;
}

