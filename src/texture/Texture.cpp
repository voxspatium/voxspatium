#include "Texture.h"
#include "util/Log.h"

Texture::Texture(GLuint texID, GLenum type)
  : m_tex(texID), m_type(type)
{

}

Texture::~Texture()
{
  glDeleteTextures(1, &m_tex);
}

Texture* Texture::create2DTexture(const std::string& file)
{
  SDL_Surface* surf = IMG_Load(file.c_str());
  if (surf == nullptr) {
    fatalError("File @" + file + " not found!");
    return nullptr;
  }
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
  SDL_FreeSurface(surf);
  return new Texture(texID, GL_TEXTURE_2D);
}

Texture* Texture::create2DArrayTexture(const std::vector<std::string>& files, int width, int height)
{
  GLuint texID;
  int index = 0;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texID);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, files.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  for (auto & file : files)
  {
    SDL_Surface* surf = IMG_Load(file.c_str());
    if (surf == nullptr) {
      fatalError("File @" + file + " not found!");
      break;
    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index++, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);
    SDL_FreeSurface(surf);
  }
  glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  return new Texture(texID, GL_TEXTURE_2D_ARRAY);
}

void Texture::bind(GLuint unit)
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(m_type, m_tex);
}
