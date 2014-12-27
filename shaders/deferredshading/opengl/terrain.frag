#version 330 compatibility

uniform sampler2D myTexture;

in vec3 in_position;
in vec3 in_normal;
in vec2 in_texcoord;

layout(location=0) out vec3 Diffuse;
layout(location=1) out vec3 Normal;
layout(location=2) out vec4 Position;

vec3 packNormal(vec3 normal) {
	return normal * 0.5 + 0.5;
}

void main (void)
{
	/*vec3 hole = vec3(512.0, 0, 512.0);
	
	if (length(in_position.xyz - hole) < 100)
		discard;*/
	
	Diffuse = texture2D(myTexture, in_texcoord).rgb;
	Normal = normalize(in_normal) * 0.5 + 0.5;
	Position = vec4(in_position, gl_FragCoord.z);
	
    //gl_FragColor = vec4(1,1,1,1);
}
