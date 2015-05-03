#version 440 core

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float Time;

in vec3 Position;
in vec2 TexCoord;

smooth out vec2 FragTexCoord;

void main()
{
    FragTexCoord = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
