uniform float height;
uniform vec4 color;
uniform vec2 currentPos;

void main() {
    float x = (currentPos.y - gl_FragCoord.y) / height;

    if (x < 0 || x > 1) {
        gl_FragColor = vec4(0, 0, 0, 1);
    } else {
        float factor = 2 * sqrt(x * (1 - x));
        gl_FragColor = vec4(factor * vec3(gl_FragColor), color.a);
    }
}
