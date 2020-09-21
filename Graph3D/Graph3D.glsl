#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

out vec2 coord;

void main()
{
	coord = aPos.xy;
	gl_Position = u_ViewProj * u_Model * vec4(aPos, 1.0f);
}

#shader fragment
#version 450 core

uniform vec4 u_Color;
uniform int u_Grid; //0 or 1

in vec2 coord;
out vec4 FragColor;

void main()
{	
	if (u_Grid == 1)
	{
		vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
		float line = min(grid.x, grid.y);
		FragColor = vec3(1.0 - min(line, 1.0)) == vec3(0.0) ? u_Color : vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		FragColor = u_Color;
	}
}