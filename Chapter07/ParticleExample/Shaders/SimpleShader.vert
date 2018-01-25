#version 410

in vec3 VertexInitVel; // Particle initial velocity
in float StartTime;    // Particle "birth" time

out float Transp;  // Transparency of the particle

uniform float Time;  // Animation time
uniform vec3 Gravity = vec3(0.0,-0.05,0.0);  // world coords
uniform float ParticleLifetime;  // Max particle lifetime

uniform mat4 ModelViewProjection;

void main()
{
    // Assume the initial position is (0,0,0).
    vec3 pos = vec3(0.0);
    Transp = 0.0;

    // Particle dosen't exist until the start time
    if( Time > StartTime ) {
        float t = Time - StartTime;

        if( t < ParticleLifetime ) {
            pos = VertexInitVel * t + Gravity * t * t;
            Transp = 1.0 - t / ParticleLifetime;
        }
    }

    // Draw at the current position
    gl_Position = ModelViewProjection * vec4(pos, 1.0);
}