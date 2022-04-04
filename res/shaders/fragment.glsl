#version 450 core

layout (location = 0) out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;

uniform sampler2D uTextures[32];

void main()
{
	int i = int(vTexIndex);
	oColor = texture(uTextures[i], vTexCoord)*vColor;
}