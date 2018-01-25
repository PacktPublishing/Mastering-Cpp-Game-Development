#version 410

in vec3 texcoords;
uniform samplerCube TextureSampler;
out vec4 frag_colour;

void main () {
	frag_colour = texture (TextureSampler, texcoords);
}