#version 330
in vec3 position;
uniform vec3 thing;
void main(void)
{
	gl_Position=vec4(position, 1.0);
}
