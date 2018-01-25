#version 410

// Input vertex data, different for all executions of this shader.
in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec3 FrontColor;
out vec3 BackColor;
out vec2 UV;

struct LightInfo {
  vec4 Position; // Light position in eye coords.
  vec3 La;       // Ambient light intensity
  vec3 Ld;       // Diffuse light intensity
  vec3 Ls;       // Specular light intensity
};
uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};

uniform MaterialInfo Material;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjection;

void getCameraSpace( out vec3 norm, out vec4 position )
{
    norm = normalize( NormalMatrix * vertexNormal);
    position = ModelViewMatrix * vec4(vertexPosition_modelspace,1.0);
}

vec3 phongModel( vec4 position, vec3 norm )
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Ls * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}

void main()
{
    vec3 cameraNorm;
    vec4 cameraPosition;

    UV = vertexUV;

    // Get the position and normal in eye space
    getCameraSpace(cameraNorm, cameraPosition);

    FrontColor = phongModel( cameraPosition, cameraNorm );
    BackColor = phongModel( cameraPosition, -cameraNorm );

    gl_Position = ModelViewProjection * vec4(vertexPosition_modelspace,1.0);
}