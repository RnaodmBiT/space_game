#version 330

layout(location = 0) in vec3 vertex;

uniform mat4 transform, world;

out vec3 position, normal;

void main() {
    vec4 v = world * vec4(vertex, 1.0f);
    gl_Position = transform * v;
    position = v.xyz;
    normal = normalize(vertex); // TODO: Fix this
}
