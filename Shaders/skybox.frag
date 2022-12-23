#version 330 core

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;
uniform bool bgState;
uniform float nearClip;
uniform float farClip;

float calculateFog() {
	float dist = 500;
	float deno = 1 / (farClip - nearClip);
	float nume = farClip - dist;
	return nume * deno;
}

void main() {
	FragColor = texture(skybox, texCoords);
	if(bgState) {
		FragColor *= vec4(0.5, 2, 0.5, 1);

	}
	FragColor = mix(FragColor, vec4(1, 1, 1, 1), calculateFog());

}