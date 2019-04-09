#version 430 core
layout (location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

out vec3 Normal;//*******
out vec3 FragPos;//*******

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 MVP;


void main() {
	//gl_Position =  vec4(VertexPosition, 1.0f);
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
	Normal = mat3(transpose(inverse(model))) * VertexNormal;//*****
	FragPos = vec3(model * vec4(VertexPosition, 1.0));//*****
}