/**
 * @file    Singleton.h
 * @brief   Abstract class for singletons
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
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template<typename C>
class Singleton {
	public:
	/**
	 * @return The instance of C
	**/
	inline static C& getInstance() {
		static C I;
		return I;
	}

	protected:
	// Prevents construction...
	Singleton() {}
	// ...as well as copy construction...
	Singleton(const C&) {}
	Singleton& operator=(const C&) {}
	// ...or premature destruction.
	virtual ~Singleton() {}

};
#endif // __SINGLETON_H__
