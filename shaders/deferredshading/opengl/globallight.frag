#version 330 compatibility

uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;

uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform vec3 AmbientColor;

void main (void)
{	
	gl_FragColor = vec4(1,0,0,1);
	//return;
	
	//vec2 texCoord = (gl_FragCoord.xy + vec2(0.5)) * invResolution;
	ivec2 texCoord = ivec2(gl_FragCoord.xy);
	
	vec3 col = texelFetch(DiffuseTex, texCoord, 0);
	vec3 normal = texelFetch(NormalTex, texCoord, 0);
	float depth = texelFetch(PositionTex, texCoord, 0).w;
	
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
