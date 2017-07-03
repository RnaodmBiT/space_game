#version 330

out vec4 color_out;

in vec3 position, normal;

uniform vec3 camera_position;
uniform vec3 albedo;

const float pi = 3.14159;

float fresnel(float f, float f0) {
    return f0 + (1 - f0) * pow(1 - f, 5);
}

float distributionGGX(float n_dot_h, float a) {
    return (a * a) / (pi * pow(n_dot_h * n_dot_h * (a * a - 1) + 1, 2));
}

float geometrySchlickGGX(float n_dot_v, float a) {
    float k = pow(a + 1, 2) / 8;
    return n_dot_v / (n_dot_v * (1 - k) + k);
}

float brdf(float n_dot_h, float n_dot_v, float n_dot_l, float roughness, float f0) {
    float d = distributionGGX(n_dot_h, roughness);
    float g = geometrySchlickGGX(n_dot_v, roughness);
    float f = fresnel(n_dot_v, f0);
    return d * f * g / (4 * n_dot_l * n_dot_v);
}

void main() {
    vec3 light = vec3(-2.0, 2.0, 2.0);
    float light_power = 20;

    vec3 eye = normalize(camera_position - position);
    vec3 h = normalize(normalize(light - position) + eye);

    float n_dot_l = dot(normal, normalize(light - position));
    float n_dot_h = dot(normal, h);
    float n_dot_v = dot(normal, eye);

    float intensity = light_power / pow(length(light - position), 2);

    float f0 = 0.1;
    float roughness = 0.9;

    vec3 diffuse = albedo / pi;
    vec3 specular = vec3(1, 1, 1) * brdf(n_dot_h, n_dot_v, n_dot_l, roughness, f0);

    float f = fresnel(dot(eye, h), f0);

    vec3 color = intensity * n_dot_l * mix(diffuse, specular * vec3(1, 1, 1), f);
    color_out = vec4(color, 1);
}
