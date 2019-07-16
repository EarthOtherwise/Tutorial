#version 330 core

in vec3 TexCoord0;

uniform samplerCube cubemapTexture;

out vec4 colour;

void main()
{
	colour = texture(cubemapTexture, TexCoord0);
}