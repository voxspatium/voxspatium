#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "util/Common.h"

template <typename AT, typename A>
struct BufferAttribute
{
	std::string name;
	std::vector<AT, A> array;
	int itemSize;
	int count;
	bool normalized;
	GLenum usage = GL_STATIC_DRAW;
	GLuint m_vb;

	BufferAttribute(const std::string& name, const std::vector<AT,A>& array, int itemSize)
		: name(name), array(array), itemSize(itemSize)
	{
		this->count = array.size() / itemSize;
		glCreateBuffers(1, &m_vb);
		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AT) * array.size(), &(array[0]), usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	}
};

class Geometry
{
	private:
		//std::map<std::string, BufferAttribute> m_attributes;
	public:

		Geometry();
		~Geometry();

};
#endif
