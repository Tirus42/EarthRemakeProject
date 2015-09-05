#version 130

uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;

//uniform vec3 CameraPos;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform float strength;
uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadAttenuation;

vec3 unpackNormal(vec3 normal) {
	return (normal - 0.5) * 2;
}

void main (void) {
	gl_FragColor = vec4(LightColor, 1);
	//return;
	
	ivec2 texCoord = ivec2(gl_FragCoord.xy);

	//vec3 position = texture(PositionTex, texCoord).rgb;
	vec3 position = texelFetch(PositionTex, texCoord, 0).rgb;
	
	// Farbe des Untergrundes auf welchen das Licht leutet
	vec3 groundColor = texelFetch(DiffuseTex, texCoord, 0).rgb;
	
	// Normale der Oberfläche holen
	vec3 normal = unpackNormal(texelFetch(NormalTex, texCoord, 0).rgb);
	
	float distance = length(position - LightPosition);
	
	vec3 lightDirection = normalize(LightPosition - position);
	
	float diffuseFactor = dot(normal, lightDirection);
	
	if (diffuseFactor < 0)
		discard;
	
	//float lightStrength = strength * 5 / (distance * distance + 1);
	
	//float lightStrength = 1.f - (distance / strength);
	//float lightStrength = 1.f - (distance / strength);
	//float lightStrength = max(0, cos(distance * 1.570796f / strength));
	//float lightStrength = 1.f - ((distance * distance) / (strength * strength));
	
	/*
	float lightStrength = max(0, 1.f - (distance / strength));
	/*/
	float lightStrength = max(0, (strength * 10 / (distance * distance) - 0.2));
	//*/
	
	//float lightStrength = max(0, cos(distance * 1.570796f / (strength * 0.75)));
	
	if (lightStrength <= 0.00) {
		//gl_FragColor = vec4(1,0,0,1);
		//return;
	}
	
	/*float lightStrength = strength / (constantAttenuation 
		+ distance * linearAttenuation
		+ distance * distance * quadAttenuation);*/
	
	groundColor.rgb *= LightColor.rgb * lightStrength * lightStrength;
	groundColor.rgb *= diffuseFactor;
	//groundColor.a = 1;
	//groundColor.a = lightStrength * diffuseFactor;
	//groundColor.rgb = LightColor;
	//groundColor = vec4(position, 1);
	
	gl_FragColor = vec4(groundColor, 1);
	//gl_FragColor.rgb = 0;
	//gl_FragColor.r = lightStrength * diffuseFactor;
}
