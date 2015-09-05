#version 130

in vec3 InPosition;

void main (void)
{
    //gl_Position = vec4(InPosition, 1.0);
	gl_Position = ftransform();
}
