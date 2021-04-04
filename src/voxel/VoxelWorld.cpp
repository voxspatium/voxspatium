#include "VoxelWorld.h"

VoxelWorld::VoxelWorld(SimplexNoise* noise, const glm::vec3& origin, unsigned int worldSize, unsigned int chunkSize)
  : m_noise(noise), m_origin(origin), m_worldSize(worldSize), m_chunkSize(chunkSize)
{
  for (uint x = 0; x < m_worldSize; x++)
  {
    for (uint z = 0; z < m_worldSize; z++)
    {
      for (uint y = 0; y < m_worldSize; y++)
      {
        m_chunks.push_back(new VoxelChunk(this, m_chunkSize, glm::vec3(x, y, z)));
      }
    }
  }

  // Set neighbors
  for (uint x = 0; x < m_worldSize; x++)
  {
    for (uint z = 0; z < m_worldSize; z++)
    {
      for (uint y = 0; y < m_worldSize; y++)
      {
        VoxelChunk* chunk = getChunkByPosi(x, y, z);
        VoxelChunk* chunkyu = getChunkByPosi(x, y + 1, z);
        VoxelChunk* chunkyd = getChunkByPosi(x, y - 1, z);
        VoxelChunk* chunkxu = getChunkByPosi(x + 1, y, z);
        VoxelChunk* chunkxd = getChunkByPosi(x - 1, y, z);
        VoxelChunk* chunkzu = getChunkByPosi(x, y, z + 1);
        VoxelChunk* chunkzd = getChunkByPosi(x, y, z - 1);

        if (chunkyu != NULL)
          chunk->connectNeighbor(VoxelChunkSide::SIDE_TOP, chunkyu);
        if (chunkyd != NULL)
          chunk->connectNeighbor(VoxelChunkSide::SIDE_BOTTOM, chunkyd);
        if (chunkxu != NULL)
          chunk->connectNeighbor(VoxelChunkSide::SIDE_RIGHT, chunkxu);
        if (chunkxd != NULL)
          chunk->connectNeighbor(VoxelChunkSide::SIDE_LEFT, chunkxd);
        if (chunkzu != NULL)
          chunk->connectNeighbor(VoxelChunkSide::SIDE_FRONT, chunkzu);
        if (chunkzd != NULL)
          chunk->connectNeighbor(VoxelChunkSide::SIDE_BACK, chunkzd);
      }
    }
  }
}

VoxelWorld::~VoxelWorld()
{
  m_chunks.clear();
}

void VoxelWorld::update(Camera* camera)
{
  m_chunksToGenerate.clear();
  m_chunksToLoad.clear();
  m_chunksToUnload.clear();
  m_chunksToRender.clear();

  for (auto & chunk : m_chunks)
  {
    // Check for chunks we haven't generated yet
    if (!chunk->isGenerated() && m_chunksToGenerate.size() < 5)
    {
      m_chunksToGenerate.push_back(chunk);
      continue;
    }
    // Check for chunks that should be loaded or unloaded
    if (chunk->isGenerated())
    {
      glm::vec3 chunki = (chunk->getIndex() * (float)m_chunkSize) + m_origin;
      // Chunk should be loaded
      float distance = glm::abs(glm::distance(chunki, camera->getPosition()));
      if (distance < 128.0f)
      {
        if (!chunk->isMeshed() || chunk->isDirty()) {
          if (m_chunksToGenerate.size() < 5)
          {
            m_chunksToLoad.push_back(chunk);
          }
        } else {
          m_chunksToRender.push_back(chunk);
        }
      }
      else if (chunk->isMeshed())
      {
        m_chunksToUnload.push_back(chunk);
      }
    }
  }

  // Generate chunks
  if (m_chunksToGenerate.size() > 0)
  {
    for (auto & chunk : m_chunksToGenerate)
    {
      chunk->generate();
    }
    return;
  }

  // Load chunks
  if (m_chunksToLoad.size() > 0)
  {
    for (auto & chunk : m_chunksToLoad)
    {
      chunk->load();
    }
    return;
  }

  // Unload chunks
  if (m_chunksToUnload.size() > 0)
  {
    for (auto & chunk : m_chunksToUnload)
    {
      chunk->unload();
    }
    return;
  }
}

VoxelChunk* VoxelWorld::getChunkByPosi(unsigned int x, unsigned int y, unsigned int z)
{
  return getChunkByPosv(glm::vec3(x, y, z));
}

VoxelChunk* VoxelWorld::getChunkByPosv(const glm::vec3& v)
{
  for (auto & chunk : m_chunks)
  {
    if (chunk->getIndex() == v)
    {
      return chunk;
    }
  }
  return NULL;
}

void VoxelWorld::render(Shader* shader, Camera* camera)
{
  shader->use();
  camera->shaderViewProjection(*shader);
  for (auto & chunk : m_chunksToRender)
  {
    chunk->render(shader);
  }
}
