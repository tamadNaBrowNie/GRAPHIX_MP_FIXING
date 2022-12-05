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
uniform vec3 dir_amb_col;
uniform vec3 dir_target;
uniform vec3 dir_color;
uniform float dir_phong;
uniform float dir_spec_str;

void dirLight(out vec3 sun, in vec3 normals){
	vec3 norm = normalize(normals);

	vec3 direction = normalize(-dir_target);

	float diff = max(dot(normals,direction),0.f);


	diff = diff+ dir_amb_str;
	vec3 dAmb = dir_amb_col *diff;

	vec3 viewDir = normalize( fragPos - eyePos);

	vec3 reflection = reflect(-direction,norm);

	float spec = pow(max (dot (reflection,viewDir),0),dir_phong);

	vec3 speccol = dir_spec_str *spec*dir_color;
	
	sun = dir_lumens*(dAmb+speccol);
}	



uniform vec3 pt_src;
uniform vec3 pt_target;

void ptLight(out vec3 bulb, in vec3 norms){

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
	vec3 norms = normCoord;
	vec3 sun;
	dirLight(sun,normal);
	
	FragColor = pixelColor *vec4(sun,1);
}