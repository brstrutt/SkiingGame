#version 430

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 position;
in vec4 inNormal;
in vec2 inUV;

out vec4 outNormal;
out vec2 outUV;

void main()
{	
	mat4 moddedView = view;

	moddedView[3][0] = 0.0f;
	moddedView[3][1] = 0.0f;
	moddedView[3][2] = 0.0f;
	moddedView[3][3] = 1.0f;

	gl_Position = projection*moddedView*position;

	outNormal = inNormal;

	outUV = inUV;
}
