/**
 * @file    Input.cpp
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
#include "Input.h"

// Remember previous positions
void Input::flush()
{
	for(auto& it : m_keyMap)
	{
		m_oldKeyMap[it.first] = it.second;
	}

	// Keep old mouse coordinates
	m_oldMouseCoords = m_mouseCoords;

	// Reset the mouse wheel scroll, as we will ever only need it once.
	m_mwheelY = 0;
	m_mwheelX = 0;
}

void Input::pressKey(unsigned int keyID)
{
	m_keyMap[keyID] = true;
}

void Input::releaseKey(unsigned int keyID)
{
	m_keyMap[keyID] = false;
}

void Input::setMouseCoords(float x, float y)
{
	m_mouseCoords.x = x;
	m_mouseCoords.y = y;
}

bool Input::isKeyDown(unsigned int keyID)
{
	auto it = m_keyMap.find(keyID);
	if(it != m_keyMap.end())
		return it->second;
	else
		return false;
}

bool Input::wasKeyDown(unsigned int keyID)
{
	auto it = m_oldKeyMap.find(keyID);
	if(it != m_oldKeyMap.end())
		return it->second;
	else
		return false;
}

bool Input::isKeyPressed(unsigned int keyID)
{
	if(isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
		return true;
	return false;
}

void Input::setMouseWheel(int vert, int horiz)
{
	m_mwheelY = vert;
	m_mwheelX = horiz;
}
