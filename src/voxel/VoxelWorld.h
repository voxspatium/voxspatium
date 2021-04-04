/**
 * @file    VoxelWorld.h
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
#ifndef __VoxelWorld_H__
#define __VoxelWorld_H__
#include "util/Common.h"
#include "util/SimplexNoise.h"
#include "Camera.h"
#include "Shader.h"
#include "VoxelChunk.h"

class VoxelWorld
{
  public:
    VoxelWorld(SimplexNoise* noise, const glm::vec3& origin, unsigned int worldSize = 16, unsigned int chunkSize = 16);
    ~VoxelWorld();

    void update(Camera* camera);
    void render(Shader* shader, Camera* camera);

    VoxelChunk* getChunkByPosi(unsigned int x, unsigned int y, unsigned int z);
    VoxelChunk* getChunkByPosv(const glm::vec3& v);

    glm::vec3 getOrigin() const { return m_origin; };

    inline unsigned int getWorldSize() const { return m_worldSize; };
    inline SimplexNoise* getNoise() const { return m_noise; };

  private:
    SimplexNoise* m_noise;
    std::vector<VoxelChunk*> m_chunks;

    std::vector<VoxelChunk*> m_chunksToGenerate;
    std::vector<VoxelChunk*> m_chunksToLoad;
    std::vector<VoxelChunk*> m_chunksToUnload;
    std::vector<VoxelChunk*> m_chunksToRender;

    glm::vec3 m_origin;
    unsigned int m_worldSize;
    unsigned int m_chunkSize;
};
#endif // __VoxelWorld_H__
