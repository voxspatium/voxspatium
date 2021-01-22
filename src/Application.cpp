/**
 * @file    Application.cpp
 * @brief   Voxspatium engine initialization
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

#include "Application.h"
#include "util/Log.h"
#include "Shader.h"

/* TEMPORARY TEST CODE */

float vertices[] = {
	10.0f, 0.0f,  10.0f,  // top right
	10.0f, 0.0f, -10.0f,  // bottom right
	-10.0f, 0.0f, -10.0f, // bottom left
	-10.0f, 0.0f,  10.0f   // top left
};

unsigned int indices[] = {
	0, 1, 3, 1, 2, 3
};

/* END OF TEMPORARY TEST CODE */

Application::Application() : m_width(1080), m_height(720)
{

}

Application::~Application()
{

}

void Application::initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	// Create our window centered at 1080x720 resolution
	m_window = SDL_CreateWindow(
		"Voxspatium Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_width,
		m_height,
		SDL_WINDOW_OPENGL
	);

	if (!m_window)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	// Set GL Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_glContext = SDL_GL_CreateContext(m_window);

	if(m_glContext == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}

	SDL_GL_MakeCurrent(m_window, m_glContext);

	// Initialize GLEW
	glewInit();

	// Create camera
	m_camera = new Camera(glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f);

	// Run the engine
	run();
}

void Application::handleEvents()
{
	/* Update the input manager */
	Input::getInstance().flush();

	/* Check for events */
	SDL_Event e;
	while ( SDL_PollEvent(&e) )
	{
		switch(e.type)
		{
			case SDL_QUIT:
				exit();
				break;
			case SDL_KEYDOWN:
				Input::getInstance().pressKey(e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				Input::getInstance().releaseKey(e.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				Input::getInstance().setMouseCoords(e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				Input::getInstance().pressKey(e.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				Input::getInstance().releaseKey(e.button.button);
				break;
			case SDL_MOUSEWHEEL:
				Input::getInstance().setMouseWheel(e.wheel.y, e.wheel.x);
				break;
		}
	}

	glm::vec2 mousepos = Input::getInstance().getMouseCoords();
	// Force mouse to the center of the screen
	SDL_SetWindowGrab(m_window, SDL_TRUE);
	SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);

	// Handle Camera Movement
	if (mousepos.x != 0 && mousepos.y != 0) {
		m_camera->processMouseMovement(-(m_width/2 - mousepos.x), m_height/2 - mousepos.y, GL_TRUE);
	}

	// Handle Camera Movement Keys
	if(Input::getInstance().isKeyDown(SDLK_w))
		m_camera->processKeyboard(Camera_Movement::FORWARD, 0.01f);

	if(Input::getInstance().isKeyDown(SDLK_s))
		m_camera->processKeyboard(Camera_Movement::BACKWARD, 0.01f);

	if(Input::getInstance().isKeyDown(SDLK_d))
		m_camera->processKeyboard(Camera_Movement::RIGHT, 0.01f);

	if(Input::getInstance().isKeyDown(SDLK_a))
		m_camera->processKeyboard(Camera_Movement::LEFT, 0.01f);

	// Handle Camera Zoom
	m_camera->processMouseScroll((float) Input::getInstance().getMouseWheelVertical() / 10.0f);

	// Print mouse position on click
	if(Input::getInstance().isKeyPressed(SDL_BUTTON_LEFT))
		std::cout << "mX: " << mousepos.x << " mY: " << mousepos.y << std::endl;

	// Toggle wireframe
	if(Input::getInstance().isKeyPressed(SDLK_x))
		m_wireframe = !m_wireframe;

	// Exit game on Esc
	if(Input::getInstance().isKeyPressed(SDLK_ESCAPE))
		exit();
}


void Application::run()
{
	m_run = true;
	m_wireframe = false;
	m_now = SDL_GetPerformanceCounter();
	m_last = 0;

	/* TEMPORARY TEST CODE */
	GLuint m_vao, m_vbo, m_ebo;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader& testShader = Shader::createShader("data/shaders/test.vert", "data/shaders/test.frag");
	testShader.linkShaders();

	// Set attribute arrays
	testShader.setAttribute("position", 3, GL_FALSE, 3, 0, GL_FLOAT);
	/* END OF TEMPORARY TEST CODE */

	while(m_run)
	{
		m_last = m_now;
		m_now = SDL_GetPerformanceCounter();

		handleEvents();

		// Clear color buffer
		glClearColor(0.39f, 0.58f, 0.93f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate time of previous frame
		deltaTime = ((m_now - m_last) / (double)SDL_GetPerformanceFrequency());

		// Enable wireframe rendering
		if (m_wireframe)
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		glEnable(GL_DEPTH_TEST | GL_CULL_FACE);
    glCullFace(GL_BACK);

		/* TEMPORARY TEST CODE */
		testShader.setBuffers(m_vao, m_vbo, m_ebo);
		testShader.use();

		m_camera->shaderViewProjection(testShader);

		testShader.setUniform("modelMatrix", glm::mat4(1.0f));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/* END OF TEMPORARY TEST CODE */

		// Disable wireframe rendering
		if (m_wireframe)
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		update(deltaTime);
		render();

		// Update window with OpenGL rendering
		SDL_GL_SwapWindow(m_window);
	}

	// After loop exits

	// Destroy window
	SDL_DestroyWindow(m_window);
	m_window = NULL;

	// Destroy context
	SDL_GL_DeleteContext(m_glContext);

	// Quit SDL subsystems
	SDL_Quit();
}

void Application::update(GLfloat dtime)
{

}

void Application::render()
{

}
