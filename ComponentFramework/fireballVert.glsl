#version 330
in  vec4 vVertex;
in  vec4 vNormal;
in  vec4 vColor;

out vec3 vertNormal;
out vec3 lightDir;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

uniform vec3 lightPos;

uniform uint elapsedTime;
uniform float deltaTime;
uniform sampler3D noiseTexture;

void main() {
	gl_PointSize = 2.0;

	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	vertNormal = normalMatrix * vNormal.xyz; /// Rotate the normal to the correct orientation 
	vec4 eyePos = modelViewMatrix * vVertex; /// Create the eye vector 
	lightDir = normalize(lightPos - eyePos.xyz); /// Create the light direction 

	vec4 temp = texture(noiseTexture, (vec3(elapsedTime)));

	float noiseLength = length(temp);

	vec4 vVertexTemp = vVertex;
	vec4 vNormalTempNormalized = normalize(vNormal);

//	vVertexTemp += temp;


	gl_Position = projectionMatrix * modelViewMatrix * vVertexTemp;
}