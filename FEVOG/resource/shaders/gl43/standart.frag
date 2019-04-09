#version 430 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

void main() {
	/*********************************/
	vec3 lightPos = vec3(2.0f, 1.0f, 2.0f);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 objectColor = vec3(0.0f, 0.5f, 0.0f);

	/***************************************************/
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
		//"vec3 result = ambient * objectColor;"
	/*******************************/
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0f);
	/***************************************************/

	//color =  vec4(0.0f, 0.5f ,0.0f , 1.0f);
}