#version 430

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 vertex;
in vec2 UV;

out vec3 fragColor;
out vec2 fragUV;

void main()
{
	vec3 vertexM = vertex * 1.0;
	fragUV = UV;
	//fragColor = abs( vertex );
	gl_Position = projection * view * model * vec4( vertexM , 1.0 );
}
