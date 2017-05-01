#version 430

uniform sampler2D texColour;

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

	vec2 tiledUV = outUV;

	if(tiledUV[0] > 1)tiledUV[0]-=floor(tiledUV[0]);
	if(tiledUV[1] > 1)tiledUV[1]-=floor(tiledUV[1]);
	if(tiledUV[0] < 0)tiledUV[0]+=floor(tiledUV[0]);
	if(tiledUV[1] < 0)tiledUV[1]+=floor(tiledUV[1]);

	vec4 texCol = texture(texColour, tiledUV);

	diffuse[3] = 1.0;

	outFrag = texCol * (diffuse + specular)/2;
	//outFrag = outNormal;
}
