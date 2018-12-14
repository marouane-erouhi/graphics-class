#version 330

in vec3 eyeDir;
in  vec3 vertNormal;
in  vec3 lightDir;
out vec4 fragColor;

uniform samplerCube cubeTexture;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;


void main() { 
mat4 modelViewMatrix = modelMatrix * viewMatrix;
mat3 scale = mat3(vec3(-1.0,0.0,0.0),
					  vec3(0.0,1.0,0.0),
					  vec3(0.0,0.0,1.0));
	vec3 reflection = reflect(eyeDir, vertNormal);
	vec3 refraction = refract (eyeDir, vertNormal, 1/1.6);
	reflection = vec3 (inverse (modelViewMatrix) * vec4 (reflection, 0.0));
	refraction = vec3 (inverse (modelViewMatrix) * vec4 (refraction, 0.0));

	//TODO: check if this the right way to mix these
	vec3 reflection_refraction_mix = mix(refraction, reflection, dot(vertNormal,lightDir));

	fragColor =  texture(cubeTexture, scale * reflection_refraction_mix);
}
