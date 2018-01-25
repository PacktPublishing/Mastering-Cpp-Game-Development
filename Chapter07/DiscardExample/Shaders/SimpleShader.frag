#version 410

in vec3 FrontColor;
in vec3 BackColor;
in vec2 UV;

out vec4 FragColor;

void main() {
    const float scale = 105.0;

    bvec2 toDiscard = greaterThan( fract(UV * scale), vec2(0.2,0.2) );

    if( all(toDiscard) )
        discard;
    else {
        if( gl_FrontFacing )
            FragColor = vec4(FrontColor, 1.0);
        else
            FragColor = vec4(BackColor, 1.0);
    }
}