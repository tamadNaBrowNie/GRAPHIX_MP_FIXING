#version 330 core
#define MIN 255

in vec3 fragPos;
uniform sampler2D tex0;
uniform sampler2D norm_tex;
uniform vec3 eyePos;
void attenuate(out float val, in float dist){
	//calculating attenuation modifier along with safeguards to prevent math error
	if(abs(dist)==0)
  //set to a minimum value otherwise
		val = MIN;
  
  	else
  
  //if distance between light and object is not  zero, 
  //attenuation is the minimum between the inverse 
  //of the distance squared and the MIN constant
  //this was done so the object would not shine brighter tha the source
		val = min(1/pow(dist,2),MIN);
}

uniform float dir_lumens;
uniform float dir_amb_str;
uniform vec4 dir_amb_col;
uniform vec3 dir_target;
uniform vec4 dir_color;
uniform float dir_phong;
uniform float dir_spec_str;

void dirLight(out vec3 sun, in vec3 normals){
	vec3 direction = normalize(dir_target);
	float diff = max(dot(normals,direction),0.f);
	vec3 dLight = diff*dir_color;
	vec3 viewDir = normalize( eyePos-fragPos);
	vec3 reflection = reflect(-direction,normals);
	float spec = pow(min (dot (reflection,viewDir),0.1),dir_phong);
	vec3 speccol = dir_spec_str *spec;
	vec3 dAmb = dir_amb_col * dir_amb_str;
	sun = dir_lumens*(dLight +speccol+dAmb)
}	



uniform vec3 pt_src;
uniform vec3 pt_target;

void ptLight(out vec3 outLight, in vec3 norms){

float val;
float dist = 0;
vec3 rayVec = pt_src - fragPos;
dist =  length(rayVec);
attenuate(val,dist);
bulb = val*bulb;

}

in vec2 texCoord;
in vec3 normCoord;

out vec4 FragColor;

void main(){
	vec4 pixelColor = texture(tex0, texCoord);
	
	vec3 normal = texture(norm_tex, texCoord).rgb;

	FragColor = pixelColor;
}