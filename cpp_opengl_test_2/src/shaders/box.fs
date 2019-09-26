#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos = vec3(-3.0f, 1.0f, -1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

const float ambientStrength = 0.1f;

void main()
{
	vec3 objectColor = vec3(0.0f, 1.0f, 0.0f);

	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;


	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 0.01f);
}