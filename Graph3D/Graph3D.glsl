#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0f);
}

#shader fragment
#version 450 core

uniform vec4 u_Color;

out vec4 FragColor;

void main()
{	
	FragColor = u_Color;
}