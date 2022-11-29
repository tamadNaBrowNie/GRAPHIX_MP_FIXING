#version 330 core

uniform sampler2D tex0;
uniform sampler2D norm_tex;

in vec2 texCoord;
in vec3 normCoord;

out vec4 FragColor;

void main(){
	vec4 pixelColor = texture(tex0, texCoord);
	
	vec3 normal = texture(norm_tex, texCoord).rgb;

	FragColor = pixelColor;
}