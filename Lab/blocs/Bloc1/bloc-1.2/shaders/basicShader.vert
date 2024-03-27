#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 Fcolor;

void main() {
    Fcolor = color;
    vec3 coord = vertex;
    gl_Position = vec4(coord, 1.0);
}
