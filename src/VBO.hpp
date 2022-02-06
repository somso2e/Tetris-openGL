#ifndef VBO_CLASS_HPP
#define VBO_CLASS_HPP

#include<common.hpp>


class VBO {
public:
	GLuint ID;
	VBO(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif