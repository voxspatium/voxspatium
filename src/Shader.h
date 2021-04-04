/**
 * @file    Shader.h
 * @brief   Shader loading and handling
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
#ifndef __SHADER_H__
#define __SHADER_H__

#include <fstream>
#include <map>

#include "util/Common.h"

class Shader
{
public:
	static Shader& createShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	static Shader& createShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath);

	void linkShaders();

	void setAttribute(const std::string& name, GLint size, GLboolean normalized, GLsizei stride, GLuint offset, GLenum type = GL_FLOAT);

	GLuint getUniformLocation(const std::string& uniformName);
	inline GLuint operator[](const std::string& name) { return getUniformLocation(name); }
	GLuint getAttribLocation(const std::string& attrbuteName);

	// Set uniforms
	void setUniform(const std::string& name, float x, float y, float z);
	void setUniform(const std::string& name, const glm::vec3& v);
	void setUniform(const std::string& name, const glm::dvec3& v);
	void setUniform(const std::string& name, const glm::vec4& v);
	void setUniform(const std::string& name, const glm::dvec4& v);
	void setUniform(const std::string& name, const glm::dmat4& m);
	void setUniform(const std::string& name, const glm::mat4& m);
	void setUniform(const std::string& name, const glm::mat3& m);
	void setUniform(const std::string& name, float val);
	void setUniform(const std::string& name, int val);

	void setBuffers(GLint vao, GLint vbo, GLint ebo);

	// Bind Attributes and start the shader program
	void use();
	void bindBuffers();
	void bindAttributes();

	// Start the shader program without binding attributes
	void start();
	void stop();
private:
	struct Attribute;
	bool m_valid;

	GLuint m_programID;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	GLuint m_geometryShaderID;

	std::map<std::string, GLuint> m_uniforms;
	std::map<std::string, Attribute> m_attributes;

	GLint vao, vbo, ebo;

	static void compileShader(const std::string& filePath, GLuint& id);
};

#endif // __SHADER_H__
