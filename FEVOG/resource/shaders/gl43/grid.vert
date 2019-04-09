#version 430 core
layout (location = 0) in vec3 VertexPosition;

uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * mat4(1.0) * vec4(VertexPosition, 1.0f);
}