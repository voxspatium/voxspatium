/**
 * @file    Application.h
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
#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#include "util/Common.h"
#include "util/Singleton.h"
#include "Camera.h"
#include "Input.h"

class Application : public Singleton<Application>
{
	public:
		Application();
		~Application();

		void initialize();
		void exit() { m_run = false; }

		inline glm::vec2 getScreenDimensions() const { return glm::vec2(m_width, m_height); }

		friend class Singleton<Application>;
	private:
		int m_width, m_height;

		Camera* m_camera;
		SDL_Window* m_window;
		SDL_GLContext m_glContext;

		GLuint m_now;
		GLuint m_last;
		double deltaTime;

		bool m_run;
		bool m_wireframe;
		bool m_mouselock = true;

		void handleEvents();
		void run();
		void render();
		void update(GLfloat dtime);
};
#endif // __APPLICATION_H__
