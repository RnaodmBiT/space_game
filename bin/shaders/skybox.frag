#version 330

out vec4 color_out;

in vec3 position, normal;

uniform vec3 albedo;
uniform samplerCube cube;

void main() {
    color_out = texture(cube, position);
}
