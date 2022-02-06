#ifndef VAO_CLASS_HPP
#define VAO_CLASS_HPP

#include <common.hpp>
#include "VBO.hpp"

class VAO {
public:
	GLuint ID;
	VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
		;
	void Bind();
	void Unbind();
	void Delete();
};

#endif