#include "Camera.h"

Camera::Camera() :
	_position(0.0f, 0.0f, 0.0f),
	_rotation(0.0f, 0.0f),
	_nearPlane(1.0f),
	_farPlane(1000.0f)
{
	// Model matrix is the identity
	_modelM = glm::mat4(1.0f);

	// Proj is based on preset camera settings;
	_projM = glm::perspective(_fov, _width / _height, _nearPlane, _farPlane);

	// Up Vector is always the same
	_up = { 0.0f, 1.0f, 0.0f };
}

glm::mat4 Camera::recalculateMVP()
{
	_direction = {
		cos(_rotation[0]) * cos(_rotation[1]),
		sin(_rotation[1]),
		sin(_rotation[0]) * cos(_rotation[1])
	};

	_cameraRight = glm::normalize(glm::cross(_up, _direction));

	_viewM = glm::lookAt(
		_position,
		_position + _direction,
		_up
	);

	_MVP = _modelM * _viewM * _projM;
	return _MVP;
}

void Camera::updatePosition(const std::array<float, 3>& positionDelta)
{
	//_position += positionDelta;
}

void Camera::updateRotation(const std::array<float, 2>& rotationDelta)
{
	//_rotation += rotationDelta;
}