#ifndef IBO_CLASS_HPP
#define IBO_CLASS_HPP

#include <common.hpp>

class IBO {
public:
	GLuint ID;
	IBO(std::vector<GLuint>& indices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif