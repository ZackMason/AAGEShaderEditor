#version 460 core
out vec4 FragColor;

in vec2 voPos;
in vec2 voUV;

uniform vec4 uColor; // export

const float min_dist = 0.01;
const float max_dist = 100.0;
const int max_step = 100;

float scene(vec3 p) {
    return distance(p, vec3(0,0,-10)) - 1.0;
}

float ray_march(vec3 ro, vec3 rd) {
    float dist = 0.0;

    for (int i = 0; i < max_step; i++) {
        float s_dist = scene(ro + rd * dist);
        dist += s_dist;
        if (dist > max_dist || s_dist < min_dist) {
            break;
        }
    }

    return dist/max_dist; 
}


void main()
{
    vec3 ro = vec3(0);
    vec3 rd = normalize(vec3(voUV.x - 0.5, voUV.y - 0.5, -1));

    vec3 color = uColor.rgb * ray_march(ro, rd);

    FragColor = vec4(vec3(color),1.);
}



