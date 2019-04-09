#version 430 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor = vec4(texture(ourTexture, TexCoord).rgb, 1.0f);
}