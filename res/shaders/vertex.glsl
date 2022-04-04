#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;



//out vec4 color;
uniform mat4 model;
uniform mat4 projection;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;

void main()
{
	vColor=aColor;
	vTexCoord=aTexCoord;
	vTexIndex=aTexIndex;
	gl_Position =projection * model * vec4(aPos, 0.0f, 1.0f);
}