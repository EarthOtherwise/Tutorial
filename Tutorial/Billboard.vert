#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 Perspective;
uniform mat4 CameraMatrix;
uniform mat4 ModelMatrix;

out vec2 UV;

void main()
{
	mat4 modelView = CameraMatrix * ModelMatrix;

	modelView[0][0] = 1.0; 
	modelView[0][1] = 0.0; 
	modelView[0][2] = 0.0; 

	modelView[1][0] = 0.0; 
	modelView[1][1] = 1.0; 
	modelView[1][2] = 0.0; 

	modelView[2][0] = 0.0; 
	modelView[2][1] = 0.0; 
	modelView[2][2] = 1.0; 

	gl_Position = Perspective * modelView * vec4(vertexPosition, 1.0);
	UV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}