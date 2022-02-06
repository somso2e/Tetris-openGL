#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "common.hpp"
#include "shader.hpp"
class Sprite {
public:
	Shader shader;
	Sprite(Shader &shader);
	void Draw();
private:
	void init();

};

#endif 