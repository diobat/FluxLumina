#include "Camera.h"

Camera::Camera() :
	_position(0.0f, 0.0f, 1.0f),
	_rotation(glm::pi<float>()/2.0f, 0.0f),
	_direction(0.0f, 0.0f, -1.0f),
	_nearPlane(0.1f),
	_farPlane(1000.0f),
	_height(768),
	_width(1024),
	_fov(90.0f),
	_translationSpeed(0.20f),
	_rotationSpeed(1.0f)
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

	_direction = glm::vec3{
		std::cos(_rotation.x) * std::cos(_rotation.y),
		std::sin(_rotation.y),
		-std::sin(_rotation.x) * std::cos(_rotation.y)
	};

	_cameraRight = glm::normalize(glm::cross(_up, _direction));

	_viewM = glm::lookAt(
		_position,
		_position + _direction,
		_up
	);
	
	_MVP = glm::transpose(_projM * _viewM *_modelM);
	
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


void Camera::resizeCameraPlane(const float& width, const float& height)
{
	_width = width;
	_height = height;

	_projM = glm::perspective(_fov, _width / _height, _nearPlane, _farPlane);
}

void Camera::move(relativeDirections d)
{

	switch (d)
	{
		case relativeDirections::FORWARD:
			_position += _direction * _translationSpeed;
			break;
		case relativeDirections::BACKWARD:
			_position -= _direction * _translationSpeed;
			break;
		case relativeDirections::LEFT: 
			_position -= +_cameraRight * _translationSpeed;
			break;
		case relativeDirections::RIGHT:
			_position += +_cameraRight * _translationSpeed;
			break;
		case relativeDirections::UP:
			_position += _up * _translationSpeed;
			break;
		case relativeDirections::DOWN:
			_position -= _up * _translationSpeed;
			break;

		default:
			break;

	}

}