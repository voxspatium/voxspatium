/**
 * @file    Camera.cpp
 * @brief   First-person free-flying camera
 *
 * Voxspatium, 3D game engine for creative space-themed games
 * Copyright (C) 2021  Evert "Diamond" Prants <evert.prants@lunasqu.ee>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Camera.h"

#include "util/Log.h"
#include "Application.h"

// TODO: use Roll

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat roll) :
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_movementSpeed(SPEED),
	m_mouseSensitivity(SENSITIVTY),
	m_zoom(ZOOM)
{
	m_position = position;
	m_worldUp = up;
	m_yaw = yaw;
	m_pitch = pitch;
	m_roll = roll;
	updateCameraVectors();
	updateProjection();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch, GLfloat roll) :
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_movementSpeed(SPEED),
	m_mouseSensitivity(SENSITIVTY),
	m_zoom(ZOOM)
{
	m_position = glm::vec3(posX, posY, posZ);
	m_worldUp = glm::vec3(upX, upY, upZ);
	m_yaw = yaw;
	m_pitch = pitch;
	m_roll = roll;
	updateCameraVectors();
	updateProjection();
}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::shaderViewProjection(Shader& shader)
{
	shader.setUniform("viewMatrix", getViewMatrix());
	shader.setUniform("projectionMatrix", m_projection);
}

void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = m_movementSpeed * deltaTime;

	if (direction == FORWARD)
		m_position += m_front * velocity;
	if (direction == BACKWARD)
		m_position -= m_front * velocity;
	if (direction == LEFT)
		m_position -= m_right * velocity;
	if (direction == RIGHT)
		m_position += m_right * velocity;
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw   += xoffset;
	m_pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}

		if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yoffset)
{
	if (m_zoom >= 44.0f && m_zoom <= 45.0f)
	{
		m_zoom -= yoffset;
	}

	if (m_zoom <= 44.0f)
	{
		m_zoom = 44.0f;
	}

	if (m_zoom >= 45.0f)
	{
		m_zoom = 45.0f;
	}

	updateProjection();
}

void Camera::updateProjection(void)
{
	// Recalculate the projection matrix
	glm::vec2 screenDims = Application::getInstance().getScreenDimensions();
	m_projection = glm::perspective(getFOV(), (GLfloat)screenDims.x/(GLfloat)screenDims.y, 0.1f, 1000.0f);
}

void Camera::updateCameraVectors(void)
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up    = glm::normalize(glm::cross(m_right, m_front));
}
