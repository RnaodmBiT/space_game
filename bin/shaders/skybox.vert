#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;

uniform mat4 transform, world;

out vec3 position, normal;

void main() {
    vec4 v = world * vec4(v_position, 1.0f);
    gl_Position = transform * v;

    position = v.xyz;
    normal = (world * vec4(v_normal, 0.0f)).xyz;
}
