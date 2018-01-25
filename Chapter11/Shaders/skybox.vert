#version 410

in vec3 vp;
uniform mat4 Projection, View;
out vec3 texcoords;

void main () {
	texcoords = vp;
	gl_Position = Projection * View * vec4 (vp, 1.0);
}