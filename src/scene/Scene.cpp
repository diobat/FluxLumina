#include "Scene.h"


Scene::Scene():
	activeCameraID(0),
	_ambientLight(0.2f, { 1.0f, 1.0f, 1.0f })
{
	;
}

void Scene::addCamera(std::shared_ptr<Camera> cameraToAdd)
{
	_objects.cameras.push_back(cameraToAdd);
}

void Scene::addModel(std::shared_ptr<ModelObject> modelToAdd)
{
	_objects.models.addModel(modelToAdd);
}

void Scene::addLightSource(std::shared_ptr<LightSource> lightSourceToAdd)
{

	std::shared_ptr<DirectionalLight> directionalLight  = std::dynamic_pointer_cast<DirectionalLight>(lightSourceToAdd);
	if(directionalLight)
	{
		addDirectionalLight(directionalLight);
		return;
	}

	std::shared_ptr<PointLight> pointLight = std::dynamic_pointer_cast<PointLight>(lightSourceToAdd);
	if(pointLight)
	{
		addPointLight(pointLight);
		return;
	}

	std::shared_ptr<SpotLight> spotLight = std::dynamic_pointer_cast<SpotLight>(lightSourceToAdd);
	if(spotLight)
	{
		addSpotLight(spotLight);
		return;
	}
}

void Scene::addDirectionalLight(std::shared_ptr<DirectionalLight> directionalLightToAdd)
{
	_objects.lights.directionalLights.push_back(directionalLightToAdd);
}

void Scene::addPointLight(std::shared_ptr<PointLight> pointLightToAdd)
{
	_objects.lights.pointLights.push_back(pointLightToAdd);
}

void Scene::addSpotLight(std::shared_ptr<SpotLight> spotLightToAdd)
{
	_objects.lights.spotLights.push_back(spotLightToAdd);
}

const SceneContents &Scene::getAllObjects() const
{
	return _objects;
}

const std::vector<std::shared_ptr<Camera>> &Scene::getAllCameras() const
{
	return _objects.cameras;
}

const ModelContents &Scene::getAllModels() const
{
	return _objects.models;
}

const std::vector<std::shared_ptr<ModelObject>> &Scene::getModels(unsigned int shaderIndex) const
{
	return _objects.models.getModels(shaderIndex);
}

const LightContents &Scene::getAllLights() const
{
	return _objects.lights;
}

std::vector<unsigned int> Scene::getShaderIDs() const
{
	return _objects.models.getShaderIDs();
}

std::shared_ptr<Camera> &Scene::getActiveCamera()
{
	return _objects.cameras[activeCameraID];
}

AmbientLight &Scene::getAmbientLight()
{
	return _ambientLight;
}
