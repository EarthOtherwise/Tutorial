#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColour;

uniform mat4 Projection;

out vec2 UV;
out vec4 Colour;

void main()
{
	gl_Position = Projection * vec4(vertexPosition, 1.0);
	UV = vec2(vertexUV.x, 1.0 - vertexUV.y);
	Colour = vertexColour;
}