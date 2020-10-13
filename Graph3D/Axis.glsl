#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main()
{
	gl_Position = u_ViewProj * u_Model * vec4(aPos, 1.f);
}

#shader fragment
#version 450 core

uniform vec3 u_Color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(u_Color, 1.f);
}