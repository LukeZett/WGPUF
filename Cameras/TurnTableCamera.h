#pragma once
#include "Camera.h"


typedef int32_t Key;

class TurnTableCamera : public Camera
{
	bool m_originMoving = false;
	bool m_turning = false;
	bool m_zooming = false;

	Key m_moveOriginKey = GLFW_KEY_LEFT_SHIFT;
	Key m_zoomingButton = GLFW_MOUSE_BUTTON_RIGHT;
	Key m_turningButton = GLFW_MOUSE_BUTTON_LEFT;


	float m_radius;

	glm::qua<float> m_identity = glm::qua(glm::vec3(0, 0, 0));

public:
	float p_turnSensitivity = 0.01;
	float p_zoomSensitivity = 0.01;


	TurnTableCamera(const glm::vec3& lookAt, const glm::vec3& position, float foV, float nearPlane = 0.1, float farPlane = 100) :
		Camera(lookAt, position, foV, nearPlane, farPlane),
		m_radius(glm::distance(lookAt, position)) {}

	void OnMouseClick(MouseButtonEvent& e);

	void OnMouseMove(MouseMoveEvent& e);

	void OnKeyboard(KeyboardEvent& e);

	void SetTurningMouseButton(Key button) { m_turningButton = button; };

	void SetMoveOriginKey(Key key) { m_moveOriginKey = key; };

	void SetZoomingMouseButton(Key button) { m_zoomingButton = button; };
};

inline void TurnTableCamera::OnMouseClick(MouseButtonEvent& e)
{
	if (e.button == m_turningButton)
	{
		switch (e.action)
		{
		case GLFW_PRESS:
			m_turning = true;
			WGF::Window().SetCursorMode(HiddenRaw);
			e.handled = true;
			break;
		case GLFW_RELEASE:
			m_turning = false;
			WGF::Window().SetCursorMode(Visible);
			e.handled = true;
			break;
		default:
			break;
		}
	}

	if (e.button == m_zoomingButton)
	{
		switch (e.action)
		{
		case GLFW_PRESS:
			m_zooming = true;
			e.handled = true;
			break;
		case GLFW_RELEASE:
			m_zooming = false;
			e.handled = true;
			break;
		default:
			break;
		}
	}
}

inline inline void TurnTableCamera::OnMouseMove(MouseMoveEvent& e)
{
	
	if (m_zooming)
	{
		m_radius += e.deltaPos.y * p_zoomSensitivity;
		m_radius = glm::clamp(m_radius, 0.1f, 100.f);
		m_position = m_focusPoint + glm::normalize(m_position - m_focusPoint) * m_radius;
		e.handled = true;
		return;
	}

	if (m_originMoving && m_turning)
	{
		glm::vec3 sideShift = glm::normalize(glm::cross(m_upVector, m_position - m_focusPoint));
		glm::vec3 upShift = glm::normalize(glm::cross(m_position - m_focusPoint, sideShift));
		glm::vec3 shift = (upShift * (float)e.deltaPos.y + sideShift * (float)-e.deltaPos.x) * p_turnSensitivity;
		m_focusPoint += shift;
		m_position += shift;

		e.handled = true;
		return;
	}

	if (m_turning) {

		auto p = glm::rotate(m_identity, (float)-e.deltaPos.x * p_turnSensitivity, m_upVector);
		p = glm::rotate(p, (float)e.deltaPos.y * p_turnSensitivity, glm::normalize(glm::cross(m_position - m_focusPoint, m_upVector)));
		m_position = m_focusPoint + p * (m_position - m_focusPoint);

		e.handled = true;
		return;
	}


}

inline void TurnTableCamera::OnKeyboard(KeyboardEvent& e)
{
	if (e.button == m_moveOriginKey && e.action != GLFW_REPEAT)
	{
		m_originMoving = e.action == GLFW_PRESS;
		e.handled = true;
	}
}