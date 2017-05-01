#version 430

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;

uniform vec3 cameraPosition;

in vec4 position;
in vec4 inNormal;
in vec2 inUV;

out vec4 camPosition;
out vec4 lightPosition;
out vec4 pointPosition;
out vec4 outNormal;
out vec2 outUV;

void main()
{	
	vec4 movedPos = vec4(position[0] * scale, position[1] * scale,position[2] * scale, 1)* model;

	gl_Position = projection*view*movedPos;

	camPosition = vec4(cameraPosition,1);
	lightPosition = vec4(-40,18,18,1);
	pointPosition = movedPos;

	mat4 rotation = model;
	rotation[0][3] = 0.0;
	rotation[1][3] = 0.0;
	rotation[2][3] = 0.0;
	rotation[3][3] = 1.0;	
	
	outNormal = vec4(normalize(inverse(mat3(model)) * vec3(inNormal)),1);
	//outNormal = inNormal;

	outUV = inUV;
}
