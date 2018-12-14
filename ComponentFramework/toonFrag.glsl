#version 330
in  vec3 vertNormal;
in  vec3 lightDir;
out vec4 fragColor;

uniform uint elapsedTime;

void main() { 
	float t = elapsedTime / 1000.0f;

	/// I could have passed these in as Uniforms but for simplicity, 
	/// I'll just define them here: specular, diffuse, ambient for the surface material 
	const vec4 ks = vec4(0.5, 0.3, 0.3, 0.0);
	const vec4 kd = vec4(0.6, 0.8, 0.6, 0.0); /// const means it cannot be changed just like C++
	const vec4 ka = 0.1 * kd;					
	
	/// The reflect() method this most GPU expensive step in this proceedure
	/// The Blinn-Phong method is faster.   	
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	float diff = max(dot(vertNormal, lightDir), 0.0);
	float spec = max(0.0, dot(vertNormal, reflection));
	if(diff != 0.0){
		spec = pow(spec,16.0);
	}

	float dotProduct = dot(vertNormal, lightDir);

	//green toon
//	if(dotProduct <= 0.33f){
//		fragColor = vec4(0, 0.250, 0.098, 1.0f);
//	}else if(dotProduct < 0.75f && dotProduct > 0.33){
//		fragColor = vec4(0, 0.498, 0.2,1.0f);
//	}else if( dotProduct < 1.0 && dotProduct > 0.75){
//		fragColor = vec4(0, 1, 0.396,1.0f);
//	}
	
	//breathing
	float c = abs(sin(elapsedTime * 0.001f));
	if(dotProduct < c){
		fragColor = vec4(0, 0.250, 0.098, 1.0f);
	}else{
		fragColor = vec4(0, 1, 0.396,1.0f);
	}

}
