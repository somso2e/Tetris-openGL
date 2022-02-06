#include <error.hpp>

void GLAPIENTRY
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceEnum, typeEnum, severityEnum;
	switch (source) {
	case GL_DEBUG_SOURCE_API: 				sourceEnum = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: 	sourceEnum = "WINDOW_SYSTEM"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: 	sourceEnum = "SHADER_COMPILER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: 		sourceEnum = "THIRD_PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: 		sourceEnum = "APPLICATION"; break;
	case GL_DEBUG_SOURCE_OTHER: 			sourceEnum = "OTHER"; break;
	}
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				typeEnum = "ERROR";	break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	typeEnum = "DEPRECATED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	typeEnum = "UNDEFINED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_PORTABILITY:			typeEnum = "PORTABILITY"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:			typeEnum = "PERFORMANCE"; break;
	case GL_DEBUG_TYPE_MARKER:				typeEnum = "MARKER"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			typeEnum = "PUSH_GROUP"; break;
	case GL_DEBUG_TYPE_POP_GROUP:			typeEnum = "POP_GROUP";	break;
	case GL_DEBUG_TYPE_OTHER:				typeEnum = "OTHER";	break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:		 severityEnum = "HIGH"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:		 severityEnum = "MEDIUM"; break;
	case GL_DEBUG_SEVERITY_LOW:			 severityEnum = "LOW"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: severityEnum = "NOTIFICATION"; break;
	}
	std::cerr << "GL CALLBACK: [" << typeEnum << "] Source:" << sourceEnum << " Severity:" << severityEnum << " Message:" << message << std::endl;
	//if (severity!=GL_DEBUG_SEVERITY_NOTIFICATION) {
	//	__debugbreak();
	//}
}


const char* glGetErrorString(GLenum error) {
	switch (error) {
	case GL_NO_ERROR:						return "GL_NO_ERROR";
	case GL_INVALID_ENUM:					return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:					return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:				return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION:	return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY:					return "GL_OUT_OF_MEMORY";
	case GL_STACK_UNDERFLOW:				return "GL_STACK_UNDERFLOW";
	case GL_STACK_OVERFLOW:					return "GL_STACK_OVERFLOW";
	case GL_CONTEXT_LOST:					return "GL_CONTEXT_LOST";
	default:								return "UNKNOWN";
	}
}
void _glCheckErrors(const char* filename, int line) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "[ERORR] (" << glGetErrorString(err) << "): " << " " << filename << ":" << line << "\n" << std::endl;
	}

}
