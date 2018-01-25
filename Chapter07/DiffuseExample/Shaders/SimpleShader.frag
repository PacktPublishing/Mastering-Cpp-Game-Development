#version 150 

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 LightIntensity;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D TextureSampler;

void main(){

	// Output color = color of the texture at the specified UV
	//color = texture( TextureSampler, UV ).rgb;
	color = vec3(LightIntensity);
}