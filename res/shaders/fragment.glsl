#version 450 core

layout (location = 0) out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in float vIsText;

uniform sampler2D uTextures[32];

void main()
{
	int i = int(vTexIndex);
	if(vIsText == 1.0f){
		oColor = vec4(vColor.rgb, texture(uTextures[i], vTexCoord).r * vColor.a);
	}else {
		oColor = vec4(texture(uTextures[i], vTexCoord).rgb * vColor.rgb, texture(uTextures[i], vTexCoord).a * vColor.a);
	}
}