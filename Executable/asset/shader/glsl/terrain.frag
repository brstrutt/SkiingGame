#version 430

uniform sampler2D texColour1;
uniform sampler2D texColour2;

in vec4 camPosition;
in vec4 lightPosition;
in vec4 pointPosition;
in vec4 outNormal;
in vec2 outUV;

out vec4 outFrag;


vec4 GimSpecular()
{	
	/*vec4 surfaceToLight = lightPosition;
	vec4 surfaceToCamera = camPosition - pointPosition;
	
	vec4 reflectedLightVector =  reflect(normalize(surfaceToLight), outNormal);
	
	float reflectedAngle = pow(max(0.0, dot( normalize(surfaceToCamera),normalize(reflectedLightVector))),1.0f);

	vec4 specular = vec4(1.0,1.0,1.0,1.0) * reflectedAngle;

	return specular;*/

	vec4 halfwayVector = normalize(lightPosition + (-camPosition));
    float specTmp = max(dot(outNormal, halfwayVector), 1.0);
    float  specularIntensity = pow(specTmp, 1);

	vec4 specular = vec4(1.0,1.0,1.0,1.0) * specularIntensity;

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

	float ratio = outNormal[1];

	vec4 texCol1 = texture(texColour1, outUV);
	vec4 texCol2 = texture(texColour2, outUV);

	vec4 finalCol;

	if(pointPosition[1] < 15)
	{
		finalCol = texCol2;
	}
	else if(pointPosition[1] > 25)
	{
		finalCol = texCol1;
	}
	else
	{
		float diff = (25 - pointPosition[1])/10;
		finalCol = texCol2 * diff + texCol1 * (1-diff);
	}

	diffuse[3] = 1.0;

	outFrag = finalCol * (diffuse + specular)/2;
	//outFrag = outNormal;
}
