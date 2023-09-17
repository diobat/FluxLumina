#pragma once

//	STD includes
#include <vector>
#include <memory>

//	First Party includes
#include "scene\Camera.h"
#include "scene\ModelObject.h"
#include "scene\LightSource.h"

class Scene// : public I_Publisher
{
public:

	Scene();

	void addCamera(std::shared_ptr<Camera> cameraToAdd);
	void addModel(std::shared_ptr<ModelObject> modelToAdd);

	std::vector<std::shared_ptr<Camera>> &getAllCameras();
	std::vector<std::shared_ptr<ModelObject>> &getAllModels();

	std::shared_ptr<Camera> &getActiveCamera();

private:

	unsigned int activeCameraID;
	
	std::vector<std::shared_ptr<Camera>> _cameras;
	std::vector<std::shared_ptr<ModelObject>> _models;
	std::vector<std::shared_ptr<LightSource>> _lights;
};