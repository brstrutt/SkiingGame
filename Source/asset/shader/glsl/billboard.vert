#version 430

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;

uniform vec3 cameraPosition;
uniform vec3 billUp;

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
	mat4 moddedModel = transpose(model);
	moddedModel = view * moddedModel;

	moddedModel[0][0] = 1.0;
	moddedModel[0][1] = 0.0;
	moddedModel[0][2] = 0.0;

	moddedModel[1][0] = 0.0;
	moddedModel[1][1] = 1.0;
	moddedModel[1][2] = 0.0;

	moddedModel[2][0] = 0.0;
	moddedModel[2][1] = 0.0;
	moddedModel[2][2] = 1.0;	

	vec4 movedPos = moddedModel * vec4(position[0] * scale, position[1] * scale,position[2] * scale, 1);


	gl_Position = projection*movedPos;

	camPosition = vec4(cameraPosition,1);
	lightPosition = vec4(-40,18,18,1);
	pointPosition = movedPos;

	mat4 rotation = model;
	rotation[0][3] = 0.0;
	rotation[1][3] = 0.0;
	rotation[2][3] = 0.0;
	rotation[3][3] = 1.0;

	outNormal = normalize(movedPos - camPosition);

	outUV = inUV;
}
