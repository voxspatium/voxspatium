/**
 * @file    VoxelChunk.h
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
#ifndef __VoxelChunk_H__
#define __VoxelChunk_H__
#include "util/Common.h"
#include "Camera.h"
#include "Shader.h"
#include "VoxelGreedyMesh.h"

enum VoxelChunkSide
{
	SIDE_TOP, SIDE_BOTTOM,
	SIDE_LEFT, SIDE_RIGHT,
	SIDE_FRONT, SIDE_BACK
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	float index;
};

class VoxelWorld;

class VoxelChunk
{
	public:
		VoxelChunk(VoxelWorld* world, unsigned int size, const glm::vec3 index);
		~VoxelChunk();

		inline VoxelWorld* getWorld() const { return m_world; };

		inline bool isGenerated() const { return m_generated; };
		inline bool isMeshed() const { return m_meshed; };
		inline bool isDirty() const { return m_dirty; };

		inline void markDirty() { m_dirty = true; };

		inline glm::vec3 getIndex() const { return m_index; };
		inline unsigned int getChunkSize() const { return m_size; };

		void update(Camera* camera);
		void render(Shader* shader);

		void generate();
		void load();
		void unload();
		void dispose();

		void connectNeighbor(VoxelChunkSide side, VoxelChunk* chunk);
		VoxelChunk* getNeighbor(VoxelChunkSide side);

		uint getVoxelIDAt(int x, int y, int z);

	private:
		unsigned int m_size;

		int m_indices;
		GLuint m_ebo, m_vbo, m_vao;

		uint*** m_data;

    VoxelWorld* m_world;
		VoxelGreedyMesh* m_mesh;

		VoxelChunk* m_neighbors[6];

		glm::vec3 m_index;
		bool m_generated, m_meshed, m_dirty;
};
#endif // __VoxelChunk_H__
