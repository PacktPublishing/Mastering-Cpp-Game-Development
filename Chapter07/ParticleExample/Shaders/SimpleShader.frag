#version 410

in float Transp;
uniform sampler2D ParticleTex;

out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, gl_PointCoord);
    FragColor.a *= Transp;
}