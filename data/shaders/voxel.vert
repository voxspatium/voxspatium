#version 330

in vec3 position;
in vec3 normal;
out vec2 test;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void) {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	test = vec2(normal.x, normal.z);
}
