#pragma once

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

// GLM includes
#include <glm/glm.hpp>

//First party headers
#include "scene/Scene.hpp"
#include "rendering/engineModules/LightMap.hpp"

// STL headers
#include <unordered_map>
#include <memory>

// Third party headers
#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>

class GraphicalEngine;

class ShadowMap
{
public:
	friend class LightLibrary;
	E_LightType getLightType() const;
	void alignShadowMap(std::shared_ptr<LightSource> light);

	std::shared_ptr<FBO> getShadowMap() const;
	const glm::mat4& getLightSpaceMatrix(unsigned int index = 0) const;
	void setDimensions(unsigned int width, unsigned int height = 0);
private:
	void setLightType(E_LightType type);
	void setShadowBuffer(std::shared_ptr<FBO> shadowMap);
	std::vector<glm::mat4> _lightSpaceMatrix;
	std::weak_ptr<FBO> _shadowDepthBuffer;
	unsigned int _bufferWidth, _bufferHeight;
	float _nearPlane, _farPlane;
	E_LightType _lightType;
};

class LightLibrary
{
public:
	LightLibrary(GraphicalEngine* engine);

	GraphicalEngine* engine() const;

    bool prepare(const LightContents& lights);

	void alignShadowMaps(std::shared_ptr<Scene> scene);
	void renderShadowMaps(std::shared_ptr<Scene> scene);

	LightMap& getLightMap();

private:
	void lightSetup(unsigned int lightIndex, const DirectionalLight &light);
	void lightSetup(unsigned int lightIndex, const PointLight &light);
	void lightSetup(unsigned int lightIndex, const SpotLight &light);

	void renderTextureShadowMap(std::shared_ptr<Scene> scene, std::shared_ptr<LightSource> light);
	void renderCubeShadowMap(std::shared_ptr<Scene> scene, std::shared_ptr<LightSource> light);

	std::unordered_map<boost::uuids::uuid, ShadowMap,  boost::hash<boost::uuids::uuid>> _shadowMaps;
	
	LightMap _lightMap;

    // The engine currently running this manager
    GraphicalEngine* _ranFrom;
};	