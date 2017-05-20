#version 430

uniform sampler2D texColour;

in vec2 outUV;

out vec4 outFrag;

void main()
{
	outFrag = texture(texColour, outUV);
}
