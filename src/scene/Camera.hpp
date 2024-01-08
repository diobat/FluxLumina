#pragma once

// Don't to math, kids
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STD includes
#include <array>

//Debug includes
#include <iostream>

// First-party includes
#include <scene/SceneObject.hpp>
#include <util/Arithmetic.hpp>

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

	void setPosition(const std::array<float, 3>& position);
	const glm::vec3& getPosition() const;

	void setRotation(const std::array<float, 2>& rotation);
	void addRotationDelta(const std::array<float, 2>& rotationDelta);

	void resizeCameraPlane(const float& width, const float& height);

	void move(relativeDirections d);
	void rotate(const std::array<float, 2>& angleDelta);

	glm::mat4 getModelMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

private:
	bool _debugMode;

	glm::vec3 _position, _direction;

	glm::vec2 _rotation;	// {Horizontal, Vertical} in radians
	glm::vec2 _rotation_safety_bars;
	void truncateRotation();

	glm::mat4 _MVP, _modelM, _viewM, _projM;
	glm::vec3 _cameraRight, _cameraUp, _up;

	float _fov;
	float _width, _height;
	float _nearPlane, _farPlane;

	float _translationSpeed, _rotationSpeed;
};