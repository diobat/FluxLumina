#include "Scene.h"


Scene::Scene()
{



}

int Scene::initialize()
{

	Model model("res/models/alliance.obj");
	addModel(std::move(model));

	Texture texture;
	texture.load("res/models/alliance.png");
	texture.bind();


	return 1;
}


void Scene::addModel(const Model& modelToAdd)
{
	_models.push_back(modelToAdd);
}

void Scene::removeMesh()
{

}

std::vector<Model>& Scene::getAllModels()
{
	return _models;
}

