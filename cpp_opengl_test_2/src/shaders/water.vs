#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 ClipSpace;


uniform mat4 model;
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);

uniform float time;

const float waveStrength = 0.02;
const float flowWaveLength = 0.1;
const float flowDirection = 0.0;
const float flowSpeed = 0.01;
const float windWaveLength = 0.05;
const float windDirection = 70.0;
const float windSpeed = 0.03;


vec3 P(float x0, float y0)
{
	vec2 flowDir = vec2(cos(radians(-flowDirection)), sin(radians(-flowDirection)));
	vec2 windDir = vec2(cos(radians(-windDirection)), sin(radians(-windDirection)));

	vec3 result;

	float h = sin(dot(windDir, vec2(x0, y0)) * (2.0 / windWaveLength) + time * (windSpeed * (2.0 / windWaveLength)))
		    * sin(dot(flowDir, vec2(x0, y0)) * (2.0 / flowWaveLength) + time * (flowSpeed * (2.0 / flowWaveLength)));

	result.y = waveStrength * h;
			   				
	result.x = x0 - windWaveLength * h * 0.5 * windDir.x;
	result.z = y0 - windWaveLength * h * 0.5 * windDir.y;

	return result;
}


vec3 N(float x0, float y0)
{
	vec2 flowDir = vec2(cos(radians(-flowDirection)), sin(radians(-flowDirection)));
	vec2 windDir = vec2(cos(radians(-windDirection)), sin(radians(-windDirection)));

	float arg1 = dot(windDir, vec2(x0, y0)) * (2.0 / windWaveLength) + time * (windSpeed * (2.0 / windWaveLength));
	float arg2 = dot(flowDir, vec2(x0, y0)) * (2.0 / flowWaveLength) + time * (flowSpeed * (2.0 / flowWaveLength));
	float dhdx = (2.0 / windWaveLength) * windDir.x * cos(arg1) * sin(arg1)
  			   + (2.0 / flowWaveLength) * flowDir.x * cos(arg2) * sin(arg2);
	float dhdy = (2.0 / windWaveLength) * windDir.y * cos(arg1) * sin(arg1)
		       + (2.0 / flowWaveLength) * flowDir.y * cos(arg2) * sin(arg2);

	vec3 B = vec3(1 - windWaveLength * dhdx * 0.5 * windDir.x, waveStrength * dhdx, 0);
	vec3 T = vec3(0, waveStrength * dhdy, 1 - windWaveLength * dhdy * 0.5 * windDir.y);

	vec3 result = -cross(B, T);

	return result;
}


const float tiling = 1.0;


void main()
{
	vec3 local_pos = P(aPos.x, aPos.y);
	gl_Position = projection * view * model * vec4(local_pos, 1.0f);
	FragPos = local_pos;
	Normal = N(aPos.x, aPos.y);
	TexCoord = (local_pos.xz / 2.0f + 0.5) * tiling;
	ClipSpace = gl_Position;
}