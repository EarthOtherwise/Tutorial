#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 Perspective;
uniform mat4 CameraMatrix;
uniform mat4 ModelMatrix;

out vec3 TexCoord0;

void main()
{
	vec4 pos = Perspective * CameraMatrix * vec4(vertexPosition, 1.0);

	gl_Position = pos.xyww;

	TexCoord0 = vertexPosition;
}