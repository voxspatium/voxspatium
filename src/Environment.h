/**
 * @file    Environment.h
 * @brief   Environment parameters
 *
 * Voxspatium, 3D game engine for creative space-themed games
 * Copyright (C) 2020  Evert "Diamond" Prants <evert.prants@lunasqu.ee>
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
#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "util/Common.h"
#include "util/Singleton.h"
#include "Shader.h"

#define MAX_LIGHTS 4

struct Light {
	Light (glm::vec3 position, glm::vec3 color, glm::vec3 attenuation = glm::vec3(1.0, 0.0, 0.0)) :
		position(position), color(color), attenuation(attenuation) {}
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 attenuation;
};

struct SpotLight : public Light {
	SpotLight (glm::vec3 position, glm::vec3 dir, glm::vec3 color, glm::vec3 attenuation) :
		Light(position, color, attenuation), direction(dir) {}
	glm::vec3 direction;
};

struct DirectionalLight : public SpotLight {
	DirectionalLight (glm::vec3 position, glm::vec3 dir, glm::vec3 color) :
		SpotLight(position, dir, color, glm::vec3(1.0, 0.0, 0.0)) {}
};

class Environment : public Singleton<Environment>
{
	public:
		void draw (Shader* shader);

		inline void setAmbientColor (glm::vec3 color) { m_ambient = color; }
		inline void setSun (Light light) { m_sun = light; }

		friend class Singleton<Environment>;
	private:
		Light m_sun;

		// Ambient color
		glm::vec3 m_ambient;

		// Fog
		glm::vec3 m_fogColor;
		float m_fogStart;
		float m_fogEnd;

		// All lights
		std::vector<Light> m_lights;
};

#endif // __ENVIRONMENT_H__
