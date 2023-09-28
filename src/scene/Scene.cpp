#include "Scene.h"


Scene::Scene():
	activeCameraID(0),
	_ambientLight(0.1f, { 1.0f, 1.0f, 1.0f })
{
	;
}

void Scene::addCamera(std::shared_ptr<Camera> cameraToAdd)
{
	_cameras.push_back(cameraToAdd);
}

void Scene::addModel(std::shared_ptr<ModelObject> modelToAdd)
{
	_objects.push_back(modelToAdd);
}

void Scene::addLightSource(std::shared_ptr<LightSource> lightSourceToAdd)
{
	_objects.push_back(lightSourceToAdd);
}

const std::vector<std::shared_ptr<SceneObject>>& Scene::getAllObjects()
{
	return _objects;
}

const std::vector<std::shared_ptr<Camera>>& Scene::getAllCameras()
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
