/**
 * @file    VoxelGreedyMesh.h
 * @brief
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
#ifndef __VoxelGreedyMesh_H__
#define __VoxelGreedyMesh_H__
#include "util/Common.h"
#include "Shader.h"

class VoxelChunk;

class VoxelGreedyMesh
{
	public:
		VoxelGreedyMesh(VoxelChunk* chunk);
		~VoxelGreedyMesh();

		inline unsigned int getIndices() const { return m_indices; };

		void render(Shader* shader);

	private:
		unsigned int m_indices;
		GLuint m_ebo, m_vbo, m_vao;
};
#endif // __VoxelGreedyMesh_H__
