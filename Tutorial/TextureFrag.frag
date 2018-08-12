#version 330 core

in vec2 UV;
in vec4 Colour;
in vec3 VertexWorldPosition;
in vec3 VertexToCamera;
in vec3 LightToCamera;
in vec3 VertexCameraNormal;

uniform sampler2D myTextureSampler;
uniform vec3 LightPosition;
uniform mat4 ModelCameraMatrix;
uniform vec3 LightColour;
uniform float LightIntensity;

out vec4 colour;

void main()
{
	vec3 MaterialDiffuseColour = texture(myTextureSampler, UV).rgb;
	vec3 MaterialAmbientColour = vec3(0.5,0.5,0.5) * MaterialDiffuseColour;
	vec3 MaterialSpecularColour = vec3(0.3,0.3,0.3);

	float distance = length(LightPosition - VertexWorldPosition);

	vec3 vertexCameraNormal = normalize(VertexCameraNormal);
	vec3 pixelToLight = normalize(LightToCamera);
	float cosTheta = clamp( dot(vertexCameraNormal,pixelToLight),0,1);

	vec3 pixelToCamera = normalize(VertexToCamera);
	vec3 reflectionDirection = reflect(-pixelToLight,vertexCameraNormal);
	float cosAlpha = clamp( dot(pixelToCamera,reflectionDirection), 0,1);
	
	colour = vec4(MaterialAmbientColour * Colour.rgb +
	MaterialDiffuseColour * Colour.rgb * LightColour * LightIntensity * cosTheta / (distance*distance) +
	MaterialSpecularColour * LightColour * LightIntensity * pow(cosAlpha,5) /  (distance*distance),Colour.w);
}