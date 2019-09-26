#version 330 core

in vec3 FragCoord;
in vec4 ClipSpace;

out vec4 FragColor;


uniform vec3 waterColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D refractionTexture;

const float ambientStrength = 0.1;
const float specularStrength = 0.5;


void main()
{
	vec2 ndc = (ClipSpace.xy / ClipSpace.w) / 2 + 0.5;
	vec2 refractCoord = vec2(ndc.x, ndc.y);
	vec4 refractColor = texture(refractionTexture, refractCoord);

	FragColor = mix(refractColor, vec4(waterColor, 1.0), 0.6);
}