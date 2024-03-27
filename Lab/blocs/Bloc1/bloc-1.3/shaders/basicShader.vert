#version 330 core

in vec3 vertex;
uniform mat4 TG;
in vec3 color;
out vec3 fcolor;

void main() {
    fcolor = color;
    gl_Position = TG * vec4(vertex, 1.0);
}
