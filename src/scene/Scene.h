#pragma once

//	STD includes
#include <vector>
#include <memory>

//	First Party includes
#include "scene\Camera.h"
#include "scene\ModelObject.h"
#include "scene\LightSource.h"
#include "util\listenerPattern.h"

class Scene// : public I_Publisher
{
public:

	Scene();

	int initialize();
	void addModel(std::shared_ptr<ModelObject> modelToAdd);
	void removeMesh();

	std::vector<std::shared_ptr<Camera>> &getAllCameras();
	std::vector<std::shared_ptr<ModelObject>> &getAllModels();

	std::shared_ptr<Camera> &getActiveCamera();

	// void BindListener(I_Listener *listener);
	// void UnbindListener(I_Listener *listener);
	// void notify(E_EventType event = E_EventType::NoEvent, std::any *object = nullptr);

private:

	unsigned int activeCameraID;
	
	std::vector<std::shared_ptr<Camera>> _cameras;
	std::vector<std::shared_ptr<ModelObject>> _models;
	std::vector<std::shared_ptr<LightSource>> _lights;
};