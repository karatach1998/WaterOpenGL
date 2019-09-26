#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 ClipSpace;

out vec4 FragColor;


uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 waterColor = vec3(0.65, 0.77, 0.82);
uniform vec3 viewPos;

const float ambientStrength = 0.1;
const float specularStrength = 0.5;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

const float distortionStrength = 0.05;


void main()
{
	vec4 normalMapColor = texture(normalMap, TexCoord);
	vec3 norm = normalize(Normal * 12.5 * vec3(-normalMapColor.r * 2.0 + 1.0, normalMapColor.b, -normalMapColor.g * 2.0 + 1.0));

	vec2 ndc = (ClipSpace.xy / ClipSpace.w) / 2 + 0.5;
	vec2 refractTexCoord = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);

	vec2 distortion1 = texture(dudvMap, ndc.xy + norm.xz).rg - 0.5;

	reflectTexCoord += 0.1 * distortion1;

	vec4 reflectionColor = texture(reflectionTexture, reflectTexCoord);
	vec4 refractionColor = texture(refractionTexture, refractTexCoord);

	vec3 resultColor = mix(vec3(mix(reflectionColor, refractionColor, 0.5)), waterColor, 0.6);

	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * mix(reflectionColor, refractionColor, 0.5);;
	FragColor = vec4(result, 1.0);
	// FragColor = mix(mix(reflectionColor, refractionColor, 0.0), vec4(result, 1.0), 0.1);
	// FragColor = texture(dudvMap, TexCoord);
	FragColor = mix(refractionColor, normalMapColor, 0.5);
	FragColor = refractionColor;
	FragColor = vec4(mix(result, waterColor, 0.7), 1.0);
}