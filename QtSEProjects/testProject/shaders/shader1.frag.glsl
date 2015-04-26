#version 150

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float Time;

uniform sampler2D AmbientMap;
uniform sampler2D DiffuseMap;

smooth in vec2 FragTexCoord;

void main()
{
    //vec3 ambientColor = texture2D(AmbientMap, FragTexCoord.xy).rgb;
    vec4 diffuseColor = texture2D(DiffuseMap, FragTexCoord.xy).rgba;

    if (diffuseColor.a == 0.0) {
        discard;
    }

    gl_FragColor = vec4(diffuseColor);
}
