#version 330 core

in vec2 UV;
in vec4 Colour;

out vec4 colour;

uniform sampler2D myTextureSampler;

void main()
{
	vec4 textureColour = texture(myTextureSampler, UV);
	colour = textureColour * Colour;
}