#version 150

// Input vertex data, different for all executions of this shader.
in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 LightIntensity;

// Values that stay constant for the whole mesh.
uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;

uniform mat4 ModelViewProjection;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){
	
	vec3 tnorm = normalize(NormalMatrix * vertexNormal);
	vec4 eyeCoords = ModelViewMatrix * vec4(vertexPosition_modelspace,1.0);
	vec3 s = normalize(vec3(LightPosition - eyeCoords));
	LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  ModelViewProjection * vec4(vertexPosition_modelspace,1);

	// UV of the vertex. No special space for this one.
	UV = vertexUV;

}