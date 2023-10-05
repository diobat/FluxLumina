#pragma once

//	STD includes
#include <vector>
#include <memory>
#include <unordered_map>

//	First Party includes
#include "scene\Camera.h"
#include "scene\ModelObject.h"
#include "scene\LightSource.h"
#include "scene\ModelContents.h"

struct LightContents
{
	std::vector<std::shared_ptr<DirectionalLight>> directionalLights;
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::vector<std::shared_ptr<SpotLight>> spotLights;
};

struct SceneContents
{
	std::vector<std::shared_ptr<Camera>> cameras;
	ModelContents models;
	LightContents lights;
};

class Scene
{
public:
	Scene();

	void addCamera(std::shared_ptr<Camera> cameraToAdd);
	void addModel(std::shared_ptr<ModelObject> modelToAdd);
	void addLightSource(std::shared_ptr<LightSource> lightSourceToAdd);

	const SceneContents &getAllObjects() const;
	const std::vector<std::shared_ptr<Camera>> &getAllCameras() const;
	const std::vector<std::shared_ptr<ModelObject>> &getModels(unsigned int shaderIndex) const;
	const LightContents &getAllLights() const;
	const ModelContents &getAllModels() const;

	std::vector<unsigned int> getShaderIDs() const;

	AmbientLight& getAmbientLight();
	std::shared_ptr<Camera> &getActiveCamera();

private:

	void addDirectionalLight(std::shared_ptr<DirectionalLight> directionalLightToAdd);
	void addPointLight(std::shared_ptr<PointLight> pointLightToAdd);
	void addSpotLight(std::shared_ptr<SpotLight> spotLightToAdd);

	unsigned int activeCameraID;

	SceneContents _objects;

	AmbientLight _ambientLight;
};