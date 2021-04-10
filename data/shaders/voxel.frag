#version 330

in vec3 uv;

uniform sampler2DArray texture0;

void main(void) {
	gl_FragColor = texture(texture0, uv.xyz);
}
