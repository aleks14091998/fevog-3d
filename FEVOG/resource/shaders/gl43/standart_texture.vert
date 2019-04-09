#version 430 core
layout (location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 Normal;//*******
out vec3 FragPos;//*******
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 MVP;


void main() {
	Normal = mat3(transpose(inverse(model))) * VertexNormal;//*****
	FragPos = vec3(model * vec4(VertexPosition, 1.0));//*****
	TexCoord = aTexCoord;
	//gl_Position =  vec4(VertexPosition, 1.0f);
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
}