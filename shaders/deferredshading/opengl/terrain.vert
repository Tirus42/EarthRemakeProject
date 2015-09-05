#version 130

uniform mat4 mMVP;
uniform mat4 mInvWorld;
uniform vec3 mOrigin;

out vec3 in_position;
out vec3 in_normal;
out vec2 in_texcoord;

void main(void)
{
	gl_Position = mMVP * gl_Vertex;
	
	in_position = mOrigin + gl_Vertex.xyz;
	
	vec4 normal = vec4(gl_Normal, 0.0);
	normal = mInvWorld * normal;
	normal = normalize(normal);
	
	in_normal = normal.xyz;
	in_texcoord = gl_MultiTexCoord0.st;
}
