/**
 * @file    Shader.cpp
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
#include "Shader.h"
#include "util/Log.h"

struct Shader::Attribute {
	GLint size;
	GLboolean normalized;
	GLsizei stride;
	GLuint offset;
	GLenum type;
};

/** Compile a shader from file */
void Shader::compileShader(const std::string& filePath, GLuint& id)
{
	// Load shader file
	std::ifstream shaderFile(filePath);
	if(shaderFile.fail())
	{
		fatalError("Failed to open file @"+filePath);
	}

	std::string fileContents = "";
	std::string line;

	while(std::getline(shaderFile, line))
	{
		fileContents += line + "\n";
	}

	shaderFile.close();

	const char* contentsPointer = fileContents.c_str();

	// Create the shader with the contents of the file
	glShaderSource(id, 1, &contentsPointer, nullptr);
	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);

		// Exit with failure.
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader @" + filePath + " failed to compile.");
	}
}

/** Create new shader from vertex and fragment files */
Shader& Shader::createShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	Shader* shader = new Shader();

	shader->m_programID = glCreateProgram();
	shader->m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if(shader->m_vertexShaderID == 0)
	{
		fatalError("Vertex shader failed to be created!");
	}

	shader->m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if(shader->m_fragmentShaderID == 0)
	{
		fatalError("Fragment shader failed to be created!");
	}

	Shader::compileShader(vertexShaderFilePath, shader->m_vertexShaderID);
	Shader::compileShader(fragmentShaderFilePath, shader->m_fragmentShaderID);

	return *shader;
}

/** Create new shader from vertex, fragment and geometry files */
Shader& Shader::createShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath)
{
	Shader* shader = new Shader();

	shader->m_programID = glCreateProgram();
	shader->m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if(shader->m_vertexShaderID == 0)
	{
		fatalError("Vertex shader failed to be created!");
	}

	shader->m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if(shader->m_fragmentShaderID == 0)
	{
		fatalError("Fragment shader failed to be created!");
	}

	shader->m_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	if(shader->m_geometryShaderID == 0)
	{
		fatalError("Geometry shader failed to be created!");
	}

	Shader::compileShader(vertexShaderFilePath, shader->m_vertexShaderID);
	Shader::compileShader(fragmentShaderFilePath, shader->m_fragmentShaderID);
	Shader::compileShader(geometryShaderFilePath, shader->m_geometryShaderID);

	return *shader;
}

/** Get uniform location */
GLuint Shader::getUniformLocation(const std::string& uniformName)
{
	auto it = m_uniforms.find(uniformName);
	if (it == m_uniforms.end())
	{
		// Get uniform location
		GLint r = glGetUniformLocation(m_programID, uniformName.c_str());
		if (r == GL_INVALID_OPERATION || r < 0)
		{
			logWarn("Uniform " + uniformName + " doesn't exist in program.");
		}

		// Add it to the cache
		m_uniforms[uniformName] = r;

		return r;
	}
	else
	{
		return it->second;
	}
}

GLuint Shader::getAttribLocation(const std::string& attrbuteName)
{
	GLint attrib = glGetAttribLocation(m_programID, attrbuteName.c_str());
	if (attrib == GL_INVALID_OPERATION || attrib < 0)
	{
		logWarn("Attribute " + attrbuteName + " doesn't exist in program.");
	}

	return attrib;
}

void Shader::setAttribute(const std::string& name,
								 GLint size,
								 GLboolean normalized,
								 GLsizei stride,
								 GLuint offset,
								 GLenum type)
{
	GLuint loc = getAttribLocation(name);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, size, type, normalized, stride * sizeof(GLfloat),
						(void*)(offset * sizeof(GLfloat)));

	m_attributes[name].size = size;
	m_attributes[name].normalized = normalized;
	m_attributes[name].stride = stride;
	m_attributes[name].offset = offset;
	m_attributes[name].type = type;
}

/** Link the shaders together */
void Shader::linkShaders()
{
	// Attach our shaders to our program
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);
	if (m_geometryShaderID != 0) {
		glAttachShader(m_programID, m_geometryShaderID);
	}

	// Link our program
	glLinkProgram(m_programID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_programID);

		// Don't leak shaders either.
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
		if (m_geometryShaderID != 0) {
			glDeleteShader(m_geometryShaderID);
		}

		std::printf("%s\n", &(infoLog[0]));
		fatalError("Shader linking failed!");
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);

	if (m_geometryShaderID != 0) {
		glDetachShader(m_programID, m_geometryShaderID);
		glDeleteShader(m_geometryShaderID);
	}
}

/** Bind the shader for usage */
void Shader::start()
{
	glUseProgram(m_programID);
}

/** Unbind the shader */
void Shader::stop()
{
	glUseProgram(0);
}

void Shader::setUniform(const std::string& name, float x, float y, float z)
{
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setUniform(const std::string& name, const glm::vec3& v)
{
	glUniform3fv(getUniformLocation(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const glm::dvec3& v)
{
	glUniform3dv(getUniformLocation(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const glm::vec4& v)
{
	glUniform4fv(getUniformLocation(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const glm::dvec4& v)
{
	glUniform4dv(getUniformLocation(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const glm::dmat4& m)
{
	glUniformMatrix4dv(getUniformLocation(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::setUniform(const std::string& name, const glm::mat4& m)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::setUniform(const std::string& name, const glm::mat3& m)
{
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::setUniform(const std::string& name, float val)
{
	glUniform1f(getUniformLocation(name), val);
}

void Shader::setUniform(const std::string& name, int val)
{
	glUniform1i(getUniformLocation(name), val);
}

void Shader::use()
{
	// Bind the shader
	start();

	// Bind the buffers
	bindBuffers();

	// Bind attributes
	bindAttributes();
}

void Shader::bindAttributes()
{
	// Enable all attributes
	for (auto it(m_attributes.begin()); it != m_attributes.end(); ++it)
	{
		GLuint location = getAttribLocation(it->first);
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(
			location,
			it->second.size,
			it->second.type,
			it->second.normalized,
			it->second.stride * sizeof(GLfloat),
			(void*)(it->second.offset * sizeof(GLfloat))
		);
	}
}

void Shader::bindBuffers()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Shader::setBuffers(GLint vao, GLint vbo, GLint ebo)
{
	this->vao = vao;
	this->vbo = vbo;
	this->ebo = ebo;
}
