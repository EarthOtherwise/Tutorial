#version 330 core

in vec2 UV;

uniform sampler2D myTextureSampler;

out vec4 colour;

void main()
{
	colour = texture( myTextureSampler, UV );
}