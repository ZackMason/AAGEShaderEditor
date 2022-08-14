#version 460 core
out vec4 FragColor;

in vec3 voPos;
in vec3 voNorm;
in vec2 voUV;

void main() {
    vec3 color = pow(vec3(0.9843, 0.0, 1.0), vec3(2.2));

    FragColor = vec4(color, 1.0);
}



