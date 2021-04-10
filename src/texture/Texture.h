#ifndef TEXTURE_H
#define TEXTURE_H
#include "util/Common.h"
#include <SDL_image.h>

class Texture
{
	private:
		GLuint m_tex;
		GLenum m_type;

	public:

		Texture(GLuint texID, GLenum type);
		~Texture();

		static Texture* create2DTexture(const std::string& file);
		static Texture* create2DArrayTexture(const std::vector<std::string>& files, int width, int height);

		void bind(GLuint unit = 0);

};
#endif
