#version 330

in vec3 position;
in vec3 normal;
in float index;

out vec3 uv;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void) {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	uv = vec3(dot(normal.zxy, position), dot(normal.yzx, position), index - 1.0f);
}
