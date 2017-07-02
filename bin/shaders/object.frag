#version 330

out vec4 color;

in vec3 position, normal;

uniform vec3 diffuse;

void main() {
    vec3 light = vec3(-2.0f, 5.0f, -2.0f);

    float n_dot_l = clamp(dot(normal, normalize(light - position)), 0, 1);

    color = vec4(diffuse * n_dot_l, 1.0f);
}
