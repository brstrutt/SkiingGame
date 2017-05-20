#version 430

uniform mat4 projection;

uniform vec2 coords;
uniform float scale;

in vec4 position;
in vec2 inUV;

out vec2 outUV;

void main()
{
	mat4 moddedView = mat4(
	1.0,0.0,0.0,coords[0],
	0.0,1.0,0.0,coords[1],
	0.0,0.0,1.0,-100.0,
	0.0,0.0,0.0,1.0
	);

	gl_Position = projection*transpose(moddedView)*position*scale;
	outUV = inUV;
}
