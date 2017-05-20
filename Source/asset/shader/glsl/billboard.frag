#version 430

uniform sampler2D texColour;
uniform sampler2D texColour2;

in vec4 camPosition;
in vec4 lightPosition;
in vec4 pointPosition;
in vec4 outNormal;
in vec2 outUV;

out vec4 outFrag;


vec4 GimSpecular()
{
	vec4 surfaceToLight = lightPosition;
	vec4 surfaceToCamera = camPosition - pointPosition;
	
	vec4 reflectedLightVector =  reflect(normalize(surfaceToLight), outNormal);
	
	float reflectedAngle = pow(max(0.0, dot( normalize(surfaceToCamera),normalize(reflectedLightVector))),1.0f);

	vec4 specular = vec4(1.0,1.0,1.0,1.0) * reflectedAngle;

	return specular;
}

void main()
{
	vec4 surfaceToLight = lightPosition;


	float angleToLight = max(dot(outNormal,normalize(surfaceToLight)),0);
	
	vec4 ambient = vec4(1.0,1.0,1.0,1.0);
	vec4 diffuse = vec4(1.0,1.0,1.0,1.0) * angleToLight;

	vec4 specular = GimSpecular();
	
	if(angleToLight > 0.0)
	{
		specular = GimSpecular();
	}

	diffuse[3] = 1.0f;
	
	vec4 texCol;
	
	if(length(pointPosition) < 100)
	{
		texCol = texture(texColour, outUV);
	}
	else
	{
		texCol = texture(texColour, outUV);
	}

	outFrag = texCol * (1);
	//outFrag = outNormal;
}
