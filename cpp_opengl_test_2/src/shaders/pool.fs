#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightColor = vec3(1, 1,1);
uniform vec3 lightPos = vec3(-5, 1, 0);
uniform sampler2D tex;

const float ambientStrength = 0.1f;


void main()
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * vec3(texture(tex, TexCoord));
	FragColor = vec4(result, 1.0f);
}