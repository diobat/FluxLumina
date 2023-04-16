#include "Scene.h"


Scene::Scene()
{



}

void Scene::initialize()
{

	Model model("res/models/alliance.obj");
	addModel(std::move(model));
}


void Scene::addModel(const Model& modelToAdd)
{
	_models.push_back(modelToAdd);
}

void Scene::removeMesh()
{

}

const std::vector<Model>& Scene::getAllModels()
{
	return _models;
}