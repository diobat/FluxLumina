#pragma once

// Don't to math, kids
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// STD includes
#include <array>

class Camera
{
public:
	Camera();

	glm::mat4 recalculateMVP();

	void updatePosition(const std::array<float, 3>& positionDelta);
	void updateRotation(const std::array<float, 2>& rotationDelta);

private:
	bool _debugMode;

	glm::vec3 _position, _direction;
	glm::vec2 _rotation;	// {Horizontal, Vertical}

	glm::mat4 _MVP, _modelM, _viewM, _projM;
	glm::vec3 _cameraRight, _up;

	float _fov, _width, _height, _nearPlane, _farPlane;
};