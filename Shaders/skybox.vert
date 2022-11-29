#version 330 core

layout (location = 0) in vec3 aPos;

// vec3, because Cubemap is a 3D image
out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec4 pos = projection * view * vec4(aPos, 1.0);

	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);

	texCoords = aPos;
}