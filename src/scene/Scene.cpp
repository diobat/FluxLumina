#include "Scene.h"


Scene::Scene():
	activeCameraID(0)
{

}

int Scene::initialize()
{


	// Import Model

	std::string model = "res/models/alliance.obj";
	std::string texture = "res/models/alliance.png";
	std::shared_ptr<ModelObject> model_object = std::make_shared<ModelObject>(model, texture);
	model_object->setCoordinates({5.0f, 5.0f, 0.0f});
	//addModel(std::move(model_object));
	addModel(std::make_shared<ModelObject>(model, texture));

	// Add camera
	_cameras.push_back(std::make_shared<Camera>());
	return 1;
}

void Scene::addModel(std::shared_ptr<ModelObject> modelToAdd)
{
	_models.push_back(modelToAdd);
}

void Scene::removeMesh()
{

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

// void Scene::BindListener(I_Listener* listener)
// {
// 	_boundListeners.push_back(listener);
// }

// void Scene::UnbindListener(I_Listener *listener)
// {
// 	remove(_boundListeners.begin(), _boundListeners.end(), listener);
// }

// void Scene::notify(E_EventType event, std::any* object)
// {
// 	for (auto listener : _boundListeners)
// 	{
// 		listener->callback(event, object);
// 	}
// }