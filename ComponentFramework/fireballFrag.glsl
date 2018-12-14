#version 330
in  vec3 vertNormal;
in  vec3 lightDir;
out vec4 fragColor;

uniform uint elapsedTime;
uniform sampler3D noiseTexture;

void main() {
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
	//fragColor =  ka + (diff * kd) + (spec * ks);
	vec4 temp = texture(noiseTexture, (vec3(elapsedTime)/128.0));

	vec4 a = texelFetch(noiseTexture, ivec3(1,1,1), 0);
	fragColor = vec4(a.xyz, 1.0f);
}

/*
	draw sphere as dots
	move each dot reletive to velocity and time - using pos change formula from physics
	make gravity go up

	u got smoke


*/