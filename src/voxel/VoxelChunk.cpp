#include "VoxelChunk.h"
#include "VoxelWorld.h"

VoxelChunk::VoxelChunk(VoxelWorld* world, unsigned int size, const glm::vec3 index)
  : m_world(world), m_index(index), m_size(size), m_generated(false), m_meshed(false), m_dirty(false),
    m_mesh(NULL), m_neighbors{}, m_data(NULL)
{
  m_data = new uint**[m_size];
  for (uint i = 0; i < m_size; i++)
  {
    m_data[i] = new uint*[m_size];
    for (uint j = 0; j < m_size; j++)
    {
      m_data[i][j] = new uint[m_size];
      for (uint k = 0; k < m_size; k++)
      {
        m_data[i][j][k] = 0;
      }
    }
  }
}

VoxelChunk::~VoxelChunk()
{
  dispose();
}

void VoxelChunk::update(Camera* camera)
{

}

void VoxelChunk::render(Shader* shader)
{
  if (!m_meshed)
  {
    return;
  }
  shader->setUniform("modelMatrix", glm::translate(glm::mat4(1.0f), m_world->getOrigin() + (m_index * (float)m_size)));
  m_mesh->render(shader);
}

void VoxelChunk::generate()
{
  //unsigned int worldSize = m_world->getWorldSize();
  SimplexNoise* noise = m_world->getNoise();
  for (uint x = 0; x < m_size; x++)
  {
    for (uint z = 0; z < m_size; z++)
    {
      float columnHeight = noise->fractal(8, ((m_index.x * m_size) + x) * 0.02, ((m_index.z * m_size) + z) * 0.02) * 10 + 10;
      for (uint y = 0; y < m_size; y++)
      {
        unsigned int absHeight = (m_index.y * m_size) + y;
        unsigned int block = absHeight < (unsigned int)columnHeight ? 1 : 0;
        if (block == 1 && absHeight < (unsigned int)columnHeight - 4) {
          block = 2;
        }
        m_data[x][y][z] = block;
      }
    }
  }
  m_generated = true;
}

void VoxelChunk::load()
{
  if (m_meshed && !m_dirty)
  {
    return;
  }
  unload();
  m_mesh = new VoxelGreedyMesh(this);
  m_meshed = true;
}

void VoxelChunk::unload()
{
  if (m_mesh != nullptr)
  {
    delete m_mesh;
    m_mesh = nullptr;
  }
  m_meshed = false;
  m_indices = 0;
}

void VoxelChunk::dispose()
{
  if (m_mesh != nullptr)
  {
    delete m_mesh;
  }
  if (m_data != nullptr)
  {
    for (uint i = 0; i < m_size; i++)
    {
      for (uint j = 0; j < m_size; j++)
      {
        delete[] m_data[i][j];
      }
      delete[] m_data[i];
    }
    delete[] m_data;
  }
  m_generated = false;
}

uint VoxelChunk::getVoxelIDAt(int x, int y, int z)
{
  if (x < 0 || x >= (int)m_size || y < 0 || y >= (int)m_size || z < 0 || z >= (int)m_size)
  {
    if (x < 0)
    {
      VoxelChunk* neighbor = getNeighbor(VoxelChunkSide::SIDE_LEFT);
      if (neighbor != nullptr)
      {
        return neighbor->getVoxelIDAt(m_size + x, y, z);
      }
      return 0;
    }

    if (x >= (int)m_size)
    {
      VoxelChunk* neighbor = getNeighbor(VoxelChunkSide::SIDE_RIGHT);
      if (neighbor != nullptr)
      {
        return neighbor->getVoxelIDAt(x - m_size, y, z);
      }
      return 0;
    }

    if (y < 0)
    {
      VoxelChunk* neighbor = getNeighbor(VoxelChunkSide::SIDE_BOTTOM);
      if (neighbor != nullptr)
      {
        return neighbor->getVoxelIDAt(x, m_size + y, z);
      }
      return 0;
    }

    if (y >= (int)m_size)
    {
      VoxelChunk* neighbor = getNeighbor(VoxelChunkSide::SIDE_TOP);
      if (neighbor != nullptr)
      {
        return neighbor->getVoxelIDAt(x, y - m_size, z);
      }
      return 0;
    }

    if (z < 0)
    {
      VoxelChunk* neighbor = getNeighbor(VoxelChunkSide::SIDE_BACK);
      if (neighbor != nullptr)
      {
        return neighbor->getVoxelIDAt(x, y, m_size + z);
      }
      return 0;
    }

    if (z >= (int)m_size)
    {
      VoxelChunk* neighbor = getNeighbor(VoxelChunkSide::SIDE_FRONT);
      if (neighbor != nullptr)
      {
        return neighbor->getVoxelIDAt(x, y, z - m_size);
      }
      return 0;
    }
    return 0;
  }
  return m_data[x][y][z];
}

void VoxelChunk::connectNeighbor(VoxelChunkSide side, VoxelChunk* chunk)
{
  m_neighbors[side] = chunk;
}

VoxelChunk* VoxelChunk::getNeighbor(VoxelChunkSide side)
{
  return m_neighbors[side];
}
