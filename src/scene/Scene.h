#pragma once

//	STD includes
#include <vector>
#include <memory>

//	First Party includes
#include "scene\Camera.h"
#include "scene\ModelObject.h"
#include "scene\LightSource.h"


struct LightContents
{
	std::vector<std::shared_ptr<DirectionalLight>> directionalLights;
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::vector<std::shared_ptr<SpotLight>> spotLights;
};

struct SceneContents
{
	std::vector<std::shared_ptr<Camera>> cameras;
	std::vector<std::shared_ptr<ModelObject>> models;
	LightContents lights;
};

class Scene// : public I_Publisher
{
public:

	Scene();

	void addCamera(std::shared_ptr<Camera> cameraToAdd);
	void addModel(std::shared_ptr<ModelObject> modelToAdd);
	void addLightSource(std::shared_ptr<LightSource> lightSourceToAdd);

	const SceneContents &getAllObjects() const;
	const std::vector<std::shared_ptr<Camera>> &getAllCameras() const;
	const std::vector<std::shared_ptr<ModelObject>> &getAllModels() const;
	const LightContents &getAllLights() const;

	AmbientLight& getAmbientLight();
	std::shared_ptr<Camera> &getActiveCamera();

private:

	void addDirectionalLight(std::shared_ptr<DirectionalLight> directionalLightToAdd);
	void addPointLight(std::shared_ptr<PointLight> pointLightToAdd);
	void addSpotLight(std::shared_ptr<SpotLight> spotLightToAdd);

	unsigned int activeCameraID;
	
	//std::vector<std::shared_ptr<Camera>> _cameras;
	//std::vector<std::shared_ptr<SceneObject>> _objects;

	SceneContents _objects;

	AmbientLight _ambientLight;
};