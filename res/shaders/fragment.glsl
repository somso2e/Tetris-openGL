#version 330 core

layout(location = 0) out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in float vIsText;

uniform sampler2D uTextures[32];

void main()
{
	switch (int(vTexIndex)) {
	case 0:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[0], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[0], vTexCoord).rgb * vColor.rgb, texture(uTextures[0], vTexCoord).a * vColor.a);
		}
		break;

	case 1:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[1], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[1], vTexCoord).rgb * vColor.rgb, texture(uTextures[1], vTexCoord).a * vColor.a);
		}
		break;

	case 2:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[2], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[2], vTexCoord).rgb * vColor.rgb, texture(uTextures[2], vTexCoord).a * vColor.a);
		}
		break;

	case 3:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[3], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[3], vTexCoord).rgb * vColor.rgb, texture(uTextures[3], vTexCoord).a * vColor.a);
		}
		break;

	case 4:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[4], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[4], vTexCoord).rgb * vColor.rgb, texture(uTextures[4], vTexCoord).a * vColor.a);
		}
		break;

	case 5:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[5], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[5], vTexCoord).rgb * vColor.rgb, texture(uTextures[5], vTexCoord).a * vColor.a);
		}
		break;

	case 6:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[6], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[6], vTexCoord).rgb * vColor.rgb, texture(uTextures[6], vTexCoord).a * vColor.a);
		}
		break;

	case 7:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[7], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[7], vTexCoord).rgb * vColor.rgb, texture(uTextures[7], vTexCoord).a * vColor.a);
		}
		break;

	case 8:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[8], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[8], vTexCoord).rgb * vColor.rgb, texture(uTextures[8], vTexCoord).a * vColor.a);
		}
		break;

	case 9:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[9], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[9], vTexCoord).rgb * vColor.rgb, texture(uTextures[9], vTexCoord).a * vColor.a);
		}
		break;

	case 10:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[10], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[10], vTexCoord).rgb * vColor.rgb, texture(uTextures[10], vTexCoord).a * vColor.a);
		}
		break;

	case 11:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[11], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[11], vTexCoord).rgb * vColor.rgb, texture(uTextures[11], vTexCoord).a * vColor.a);
		}
		break;

	case 12:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[12], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[12], vTexCoord).rgb * vColor.rgb, texture(uTextures[12], vTexCoord).a * vColor.a);
		}
		break;

	case 13:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[13], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[13], vTexCoord).rgb * vColor.rgb, texture(uTextures[13], vTexCoord).a * vColor.a);
		}
		break;

	case 14:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[14], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[14], vTexCoord).rgb * vColor.rgb, texture(uTextures[14], vTexCoord).a * vColor.a);
		}
		break;

	case 15:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[15], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[15], vTexCoord).rgb * vColor.rgb, texture(uTextures[15], vTexCoord).a * vColor.a);
		}
		break;

	case 16:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[16], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[16], vTexCoord).rgb * vColor.rgb, texture(uTextures[16], vTexCoord).a * vColor.a);
		}
		break;

	case 17:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[17], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[17], vTexCoord).rgb * vColor.rgb, texture(uTextures[17], vTexCoord).a * vColor.a);
		}
		break;

	case 18:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[18], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[18], vTexCoord).rgb * vColor.rgb, texture(uTextures[18], vTexCoord).a * vColor.a);
		}
		break;

	case 19:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[19], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[19], vTexCoord).rgb * vColor.rgb, texture(uTextures[19], vTexCoord).a * vColor.a);
		}
		break;

	case 20:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[20], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[20], vTexCoord).rgb * vColor.rgb, texture(uTextures[20], vTexCoord).a * vColor.a);
		}
		break;

	case 21:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[21], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[21], vTexCoord).rgb * vColor.rgb, texture(uTextures[21], vTexCoord).a * vColor.a);
		}
		break;

	case 22:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[22], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[22], vTexCoord).rgb * vColor.rgb, texture(uTextures[22], vTexCoord).a * vColor.a);
		}
		break;

	case 23:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[23], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[23], vTexCoord).rgb * vColor.rgb, texture(uTextures[23], vTexCoord).a * vColor.a);
		}
		break;

	case 24:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[24], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[24], vTexCoord).rgb * vColor.rgb, texture(uTextures[24], vTexCoord).a * vColor.a);
		}
		break;

	case 25:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[25], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[25], vTexCoord).rgb * vColor.rgb, texture(uTextures[25], vTexCoord).a * vColor.a);
		}
		break;

	case 26:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[26], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[26], vTexCoord).rgb * vColor.rgb, texture(uTextures[26], vTexCoord).a * vColor.a);
		}
		break;

	case 27:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[27], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[27], vTexCoord).rgb * vColor.rgb, texture(uTextures[27], vTexCoord).a * vColor.a);
		}
		break;

	case 28:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[28], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[28], vTexCoord).rgb * vColor.rgb, texture(uTextures[28], vTexCoord).a * vColor.a);
		}
		break;

	case 29:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[29], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[29], vTexCoord).rgb * vColor.rgb, texture(uTextures[29], vTexCoord).a * vColor.a);
		}
		break;

	case 30:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[30], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[30], vTexCoord).rgb * vColor.rgb, texture(uTextures[30], vTexCoord).a * vColor.a);
		}
		break;

	case 31:
		if (vIsText == 1.0f) {
			oColor = vec4(vColor.rgb, texture(uTextures[31], vTexCoord).r * vColor.a);
		}
		else {
			oColor = vec4(texture(uTextures[31], vTexCoord).rgb * vColor.rgb, texture(uTextures[31], vTexCoord).a * vColor.a);
		}
		break;


	}
}