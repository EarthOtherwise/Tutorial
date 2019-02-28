#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 Perspective;
uniform mat4 CameraMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition;

out vec2 UV;
out vec3 VertexWorldPosition;
out vec3 VertexToCamera;
out vec3 LightToCamera;
out vec3 VertexCameraNormal;

void main()
{
	gl_Position = Perspective * ModelMatrix * vec4(vertexPosition, 1.0);
	UV = vec2(vertexUV.x, 1.0 - vertexUV.y);

	vec3 vertexCameraPosition = (CameraMatrix * ModelMatrix * vec4(vertexPosition, 1.0)).xyz;
	VertexToCamera = (0,0,0) - vertexCameraPosition;

	vec3 lightCameraPosition = (CameraMatrix * vec4(LightPosition,1.0)).xyz;
	LightToCamera = VertexToCamera + VertexToCamera;

	VertexCameraNormal = (CameraMatrix * ModelMatrix * vec4(vertexNormal, 0)).xyz;

	VertexWorldPosition = (ModelMatrix * vec4(vertexPosition, 1.0)).xyz;
}