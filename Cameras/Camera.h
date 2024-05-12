#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "WGPUFramework.h"

class Camera
{
protected:
	glm::vec3 m_focusPoint;
	glm::vec3 m_position;
	glm::vec3 m_upVector = {0, 1, 0};
	float m_zNear;
	float m_zFar;
	float m_fov;
public:
	Camera(const glm::vec3& lookAt, const glm::vec3& position, float foV, float nearPlane = 0.1, float farPlane = 100) :
		m_focusPoint(lookAt), m_position(position), m_fov(foV), m_zNear(nearPlane), m_zFar(farPlane)
	{
	}

	glm::mat4 getViewMat() const {
		return glm::lookAt(m_position, m_focusPoint, m_upVector);
	}

	glm::mat4 getPerspectiveMat() const {
		return glm::perspective(m_fov, WGF::Window().GetAspectRatio(), m_zNear, m_zFar);
	}

	~Camera() = default;
};