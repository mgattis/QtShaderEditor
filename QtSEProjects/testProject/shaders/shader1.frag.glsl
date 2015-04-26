uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float Time;

uniform sampler2D DiffuseMap;

varying vec2 FragTexCoord;

void main()
{
    vec3 color = texture2D(DiffuseMap, FragTexCoord.xy).rgb;
    //vec3 color = vec3(1.0, 1.0, 1.0);
    gl_FragColor = vec4(color, 1.0);
}
