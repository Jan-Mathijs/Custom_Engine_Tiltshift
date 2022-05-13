#version 460 core
layout (location = 0) in vec2 a_Position;
layout (location = 1) in float a_PointSize;
layout (location = 0) uniform mat4 u_VP;

void main()
{
	gl_PointSize = a_PointSize;
	gl_Position = u_VP * vec4(a_Position, 0.0, 1.0);
}