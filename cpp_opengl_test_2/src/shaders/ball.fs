#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;


uniform vec3 lightColor;
uniform vec3 lightPos;
unifrom vec3 viewPos;
uniform sampler2D tex;

const float ambientStrength = 0.1;
const float specularStrength = 0.5;


void main()
{
	texColor = texture(tex, TexCoord);

	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * texColor;
	FragColor = vec4(result, 1.0);
}