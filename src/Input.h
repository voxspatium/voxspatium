/**
 * @file    Input.h
 * @brief   Keyboard and mouse input handling
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
#ifndef __INPUT_H__
#define __INPUT_H__

#include "util/Singleton.h"
#include "util/Math3D.h"

#include <unordered_map>

class Input : public Singleton<Input>
{
	public:
		void flush();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		void setMouseCoords(float x, float y);
		void setMouseWheel(int vert, int horiz);

		bool isKeyDown(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 getMouseCoords() const { return m_mouseCoords; }
		glm::vec2 getOldMouseCoords() const { return m_oldMouseCoords; }

		int getMouseWheelVertical() const { return m_mwheelY; }
		int getMouseWheelHorizontal() const { return m_mwheelX; }

		friend class Singleton<Input>;
	private:
		bool wasKeyDown(unsigned int keyID);
		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_oldKeyMap;
		glm::vec2 m_mouseCoords;
		glm::vec2 m_oldMouseCoords;
		int m_mwheelY;
		int m_mwheelX;
};
#endif // __INPUT_H__
