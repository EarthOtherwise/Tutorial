#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColour;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 Perspective;


out vec2 UV;
out vec4 Colour;


void main()
{
	gl_Position = Perspective * vec4(vertexPosition, 1.0);
	UV = vec2(vertexUV.x, 1.0 - vertexUV.y);
	Colour = vertexColour;
}