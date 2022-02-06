#include <common.hpp>

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

const char* glGetErrorString(GLenum error);
void _glCheckErrors(const char* filename, int line);

#define glCheckErrors() _glCheckErrors(__FILE__, __LINE__)
