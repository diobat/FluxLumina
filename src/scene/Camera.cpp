#include "Camera.h"

Camera::Camera() :
	_debugMode(false),
	_position(0.0f, 0.0f, 1.0f),
	_rotation(glm::pi<float>()/2.0f, 0.0f),
	_direction(0.0f, 0.0f, -1.0f),
	_nearPlane(0.1f),
	_farPlane(1000.0f),
	_height(768),
	_width(1024),
	_fov(70.0f),
	_translationSpeed(0.20f),
	_rotationSpeed(0.001f)
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

	std::array<float, 2> rotation_adjusted_for_speed{ std::remainderf((-rotationDelta[0] * _rotationSpeed), glm::two_pi<float>()) , std::remainder((-rotationDelta[1] * _rotationSpeed), glm::two_pi<float>()) };

	if(_debugMode)
	{
		std::cout << "Horizontal: " << rotation_adjusted_for_speed[0] << "    Vertical: " << rotation_adjusted_for_speed[1] << std::endl;
	}

	_rotation = glm::make_vec2(rotation_adjusted_for_speed.data());

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

void Camera::rotate(const std::array<float, 2>& angleDelta)
{
	_rotation += glm::make_vec2(angleDelta.data());
}
