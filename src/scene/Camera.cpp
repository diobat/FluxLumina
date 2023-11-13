#include "Camera.hpp"

Camera::Camera() : 
	_debugMode(false),
	_position(0.0f, 0.0f, 1.0f),
	_rotation(glm::pi<float>() / 2.0f, 0.0f),
	_direction(0.0f, 0.0f, -1.0f),
	_nearPlane(0.1f),
	_farPlane(5000.0f),
	_height(768),
	_width(1024),
	_fov(70.0f),
	_translationSpeed(0.20f),
	_rotationSpeed(0.001f),
	_rotation_safety_bars(-glm::pi<float>() / 2.2f, glm::pi<float>() / 2.2f)
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

	_cameraRight = glm::normalize(glm::cross(_direction, _up));
	_cameraUp = glm::normalize (glm::cross(_cameraRight, _direction));

	_viewM = glm::lookAt(
		_position,
		_position + _direction,
		_up
	);
	
	_MVP = (_projM * _viewM *_modelM);
	
	return _MVP;
}

void Camera::setPosition(const std::array<float, 3>& position)
{
	_position = glm::vec3(position[0], position[1], position[2]);
}

const glm::vec3 &Camera::getPosition() const
{
	return _position;
}

void Camera::setRotation(const std::array<float, 2>& rotation)
{

	std::array<float, 2> rotation_adjusted_for_speed{ std::remainderf((-rotation[0] * _rotationSpeed), glm::two_pi<float>()) , std::remainder((-rotation[1] * _rotationSpeed), glm::two_pi<float>()) };

	if(_debugMode)
	{
		std::cout << "Horizontal: " << rotation_adjusted_for_speed[0] << "    Vertical: " << rotation_adjusted_for_speed[1] << std::endl;
	}

	_rotation = glm::make_vec2(rotation_adjusted_for_speed.data());
}

void Camera::addRotationDelta(const std::array<float, 2>& rotationDelta)
{
	glm::vec2 speed_adjusted_rotation = {-rotationDelta[0] * _rotationSpeed, -rotationDelta[1] * _rotationSpeed};
	_rotation += speed_adjusted_rotation;

	if(_debugMode)
	{
		std::cout << "Horizontal: " << _rotation[0] << "    Vertical: " << _rotation[1] << std::endl;
	}

	truncateRotation();
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
			_position += _cameraUp * _translationSpeed;
			break;
		case relativeDirections::DOWN:
			_position -= _cameraUp * _translationSpeed;
			break;
		default:
			break;
	}
}

void Camera::rotate(const std::array<float, 2>& angleDelta)
{
	_rotation += glm::make_vec2(angleDelta.data());
}


void Camera::truncateRotation()
{
	// We only really care about truncating the Vertical Rotation
	_rotation[1] = std::max(_rotation[1], _rotation_safety_bars[0]);
	_rotation[1] = std::min(_rotation[1], _rotation_safety_bars[1]);
}

glm::mat4 Camera::getModelMatrix() const
{
	return _modelM;
}

glm::mat4 Camera::getViewMatrix() const
{
	return _viewM;
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return _projM;
}