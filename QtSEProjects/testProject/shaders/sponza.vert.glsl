#version 440 core

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform float Time;

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;
in vec4 Tangent;

smooth out vec3 FragPosition;
smooth out vec2 FragTexCoord;
smooth out vec3 FragNormal;
smooth out vec3 FragTangent;
smooth out vec3 FragBiTangent;

flat out vec3 globalLight;

void main()
{
	mat4 toEyeSpace = ViewMatrix * ModelMatrix;

    FragTexCoord = TexCoord;
    
    FragNormal = normalize(NormalMatrix * Normal);
    FragTangent = normalize(vec3(toEyeSpace * vec4(Tangent.xyz, 0.0)));
    FragBiTangent = normalize(cross(FragNormal, FragTangent) * Tangent.w);    

    globalLight = normalize(vec3(1.0, 2.0, 1.0));
    globalLight = normalize(vec3(toEyeSpace * vec4(globalLight, 0.0)));

    FragPosition = vec3(toEyeSpace * vec4(Position, 1.0));
    
    vec4 outputPosition = ProjectionMatrix * toEyeSpace * vec4(Position, 1.0);
    gl_Position = outputPosition;
}
