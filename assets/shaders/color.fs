#version 460 core
out vec4 FragColor;

in vec2 voPos;
in vec2 voUV;

uniform vec4 uColor; // export

void main()
{
    vec3 color = uColor.rgb;

    FragColor = vec4(vec3(color),1.);
}



