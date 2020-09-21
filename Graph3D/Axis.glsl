#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

out vec3 Color;

void main()
{
	gl_Position = u_ViewProj * vec4(aPos, 1.f);
	Color = aColor;
}

#shader fragment
#version 450 core

in vec3 Color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(Color, 1.f);
}