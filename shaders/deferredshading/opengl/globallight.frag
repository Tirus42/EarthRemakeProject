#version 330 compatibility

uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;

const vec2 invResolution = vec2(1.0 / 1024.0, 1.0 / 768.0);

uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform vec3 AmbientColor;

void main (void)
{	
	gl_FragColor = vec4(1,0,0,1);
	//return;
	
	vec2 texCoord = (gl_FragCoord.xy + vec2(0.5)) * invResolution;
	
	vec3 col = texture2D(DiffuseTex, texCoord).rgb;
	vec3 normal = texture2D(NormalTex, texCoord).xyz;
	float depth = texture2D(PositionTex, texCoord).w;
	
	if (col == vec3(0))
		discard;
	
	vec3 ambientColor = col.rgb * AmbientColor.rgb;
	
	float diffuseFactor = dot(normal, LightDirection);
	
	vec3 diffuserTerm = vec3(0);
	
	if (diffuseFactor > 0) {
		diffuserTerm = col * diffuseFactor * LightColor;
	}
	
	gl_FragDepth = depth;
    gl_FragColor = vec4(ambientColor + diffuserTerm, 1);
	
	//gl_FragColor = vec4(depth, depth, depth, 1);
}
