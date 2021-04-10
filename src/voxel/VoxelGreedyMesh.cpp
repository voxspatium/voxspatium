#include "VoxelGreedyMesh.h"
#include "VoxelChunk.h"
#include <algorithm>

VoxelGreedyMesh::VoxelGreedyMesh(VoxelChunk* chunk)
{
  unsigned int size = chunk->getChunkSize();
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<glm::vec3> quad;

  for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b) {
    // Sweep over 3-axes
    for (int d = 0; d < 3; ++d) {
      int i, j, k, l, w, h;
      // VoxelChunkSide side;
      int u = (d + 1) % 3;
      int v = (d + 2) % 3;
      int x[3] = {0, 0, 0};
      int q[3] = {0, 0, 0};

      // Here we're keeping track of the side that we're meshing.
      // if (d == 0) side = backFace ? VoxelChunkSide::SIDE_LEFT : VoxelChunkSide::SIDE_RIGHT;
      // else if (d == 1) side = backFace ? VoxelChunkSide::SIDE_BOTTOM : VoxelChunkSide::SIDE_TOP;
      // else side = backFace ? VoxelChunkSide::SIDE_BACK : VoxelChunkSide::SIDE_FRONT;

      signed int mask[size * size];
      q[d] = 1;
      // Move through the dimension from front to back
      for (x[d] = -1; x[d] < (int)size;) {
        // Compute mask
        int n = 0;
        for (x[v] = 0; x[v] < (int)size; ++x[v]) {
          for (x[u] = 0; x[u] < (int)size; ++x[u]) {
            uint current = chunk->getVoxelIDAt(x[0], x[1], x[2]);
            uint adjacent = chunk->getVoxelIDAt(x[0] + q[0], x[1] + q[1], x[2] + q[2]);
            mask[n++] = ((!!current) == (!!adjacent)) ? 0 : ((!!current) ? current : -adjacent);
          }
        }

        // Increment x[d]
        ++x[d];

        // Generate mesh for mask using lexicographic ordering
        n = 0;
        for (j = 0; j < (int)size; ++j) {
          for (i = 0; i < (int)size;) {
            int c = mask[n];
            if (!!c) {
              // Compute width
              for (w = 1; mask[n + w] == c && i + w < (int)size; ++w) {}

              // Compute height
              bool done = false;
              for (h = 1; j + h < (int)size; ++h) {
                for (k = 0; k < w; ++k) {
                  if (mask[n + k + h * (int)size] != c) {
                    done = true;
                    break;
                  }
                }
                if (done) break;
              }

              // Create quad
              x[u] = i;
              x[v] = j;

              int du[3] = {0, 0, 0};
              int dv[3] = {0, 0, 0};

              du[u] = w;
              dv[v] = h;

              if (c < 0) {
                c = -c;
              }

              quad.clear();
              quad.push_back({x[0], x[1], x[2]});
              quad.push_back({x[0] + du[0], x[1] + du[1], x[2] + du[2]});
              quad.push_back({x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]});
              quad.push_back({x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]});

              // Add vertices and normals
              for (int qindex = 0; qindex < 4; ++qindex) {
                vertices.push_back({
                  quad[qindex],
                  {0.0f, 0.0f, 0.0f},
                  (float)c
                });
              }

              // Add indices
              int indexi = vertices.size() - 4;
              if (backFace) {
                indices.push_back(indexi + 2);
                indices.push_back(indexi + 1);
                indices.push_back(indexi);
                indices.push_back(indexi + 3);
                indices.push_back(indexi + 2);
                indices.push_back(indexi);
              } else {
                indices.push_back(indexi);
                indices.push_back(indexi + 1);
                indices.push_back(indexi + 2);
                indices.push_back(indexi);
                indices.push_back(indexi + 2);
                indices.push_back(indexi + 3);
              }

              // Zero-out mask
              for (l = 0; l < h; ++l) {
                for (k = 0; k < w; ++k) {
                  mask[n + k + l * (int)size] = 0;
                }
              }

              // Increment counters and continue
              i += w;
              n += w;
            } else {
              ++i;
              ++n;
            }
          }
        }
      }
    }
  }

  m_indices = indices.size();

  // Generate face normals
  // TODO: move this somewhere so that it could be reused
  for (unsigned int i = 0; i < m_indices; i += 3) {
    glm::vec3 vA = vertices[indices[i]].position;
    glm::vec3 vB = vertices[indices[i + 1]].position;
    glm::vec3 vC = vertices[indices[i + 2]].position;

    glm::vec3 res = glm::cross(vC - vB, vA - vB);

    vertices[indices[i]].normal = glm::normalize(res);
    vertices[indices[i + 1]].normal = glm::normalize(res);
    vertices[indices[i + 2]].normal = glm::normalize(res);
  }

  glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

  glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices, &(indices[0]), GL_STATIC_DRAW);
}

VoxelGreedyMesh::~VoxelGreedyMesh()
{
  glDeleteVertexArrays(1, &m_vao);
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
}

void VoxelGreedyMesh::render(Shader* shader)
{
  shader->setBuffers(m_vao, m_vbo, m_ebo);
  shader->bindBuffers();
  shader->bindAttributes();
  glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, nullptr);
}
