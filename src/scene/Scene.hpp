#pragma once

//	STD includes
#include <vector>
#include <memory>
#include <unordered_map>

//	First Party includes
#include "scene/Camera.hpp"
#include "scene/ModelObject.hpp"
#include "scene/LightSource.hpp"
#include "scene/ModelContents.hpp"
#include "scene/Skybox.hpp"

// Third-party headers
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
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
	const std::vector<std::shared_ptr<ModelObject>> &getModels() const;
	std::vector<std::shared_ptr<ModelObject>> getModels(const std::string& shader) const;
	const LightContents &getAllLights() const;

	AmbientLight& getAmbientLight();
	std::shared_ptr<Camera> &getActiveCamera();
	Skybox& getSkybox();

	std::shared_ptr<SceneObject> get(const boost::uuids::uuid& id);

private:
	void addDirectionalLight(std::shared_ptr<DirectionalLight> directionalLightToAdd);
	void addPointLight(std::shared_ptr<PointLight> pointLightToAdd);
	void addSpotLight(std::shared_ptr<SpotLight> spotLightToAdd);

	unsigned int activeCameraID;

	boost::uuids::uuid _id;

	SceneContents _objects;

	AmbientLight _ambientLight;
	Skybox _skybox;
};