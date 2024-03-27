#version 330 core

out vec4 FragColor;
in vec3 Fcolor;

void main() {

    int heigth = 714;
    int density = 20;
    int step = 5;

    if(int(gl_FragCoord.y) % density < step)
        discard;

    FragColor = vec4(Fcolor, 1.0);
    /*
    if(gl_FragCoord.y < 714 / 2) {
        FragColor = vec4(1, 0, 0, 1);
        if(gl_FragCoord.x > 715 / 2)
            FragColor = vec4(0, 0, 1, 1);
    } else {
        FragColor = vec4(0, 1, 0, 1);
        if(gl_FragCoord.x < 715 / 2)
            FragColor = vec4(1, 1, 0, 1);
    }
    */
}
