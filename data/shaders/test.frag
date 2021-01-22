#version 330

in vec2 test;

void main(void) {
	gl_FragColor = vec4(0.0f, (test.x / 10.0f + 1.0f) / 2.0f, (test.y / 10.0f + 1.0f) / 2.0f, 1.0f);
}
