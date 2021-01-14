#version 410

layout (location = 0) in vec4 position;

uniform mat4 modelViewProjectionMatrix;
uniform float point_size;

void main()
{
	gl_Position = modelViewProjectionMatrix * position;
	gl_PointSize = point_size;
}
