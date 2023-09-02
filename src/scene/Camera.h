#pragma once

// Don't to math, kids
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STD includes
#include <array>

//Debug includes
//#define GLM_ENABLE_EXPERIMENTAL
//#include "glm/gtx/string_cast.hpp"
#include <iostream>

enum class relativeDirections : unsigned int
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Camera
{
public:
	Camera();

	glm::mat4 recalculateMVP();

	void updatePosition(const std::array<float, 3>& positionDelta);
	void updateRotation(const std::array<float, 2>& rotationDelta);

	void resizeCameraPlane(const float& width, const float& height);

	void move(relativeDirections d);
	void rotate(const std::array<float, 2>& angleDelta);

private:
	bool _debugMode;

	glm::vec3 _position, _direction;
	glm::vec2 _rotation;	// {Horizontal, Vertical}

	glm::mat4 _MVP, _modelM, _viewM, _projM;
	glm::vec3 _cameraRight, _up;

	float _fov, _width, _height, _nearPlane, _farPlane;

	float _translationSpeed, _rotationSpeed;
};