#version 440 core
out vec4 OutColor;

uniform mat3 NormalMatrix;

uniform vec3 MatAmbient;
uniform vec3 MatDiffuse;
uniform vec3 MatSpecular;
uniform float MatShininess;
uniform float Time;

uniform sampler2D AmbientMap;
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;

smooth in vec3 FragPosition;
smooth in vec2 FragTexCoord;
smooth in vec3 FragNormal;
smooth in vec3 FragTangent;
smooth in vec3 FragBiTangent;

flat in vec3 globalLight;

void main()
{
    vec4 rawColor = texture2D(DiffuseMap, FragTexCoord.xy).rgba;
    vec3 specularColor = texture2D(SpecularMap, FragTexCoord.xy).xyz;
    //vec3 surfaceNormal = texture2D(NormalMap, FragTexCoord.xy).xyz;
    vec3 diffuseColor = rawColor.rgb;

    if (rawColor.a < 1.0) {
    	discard;
    }

    vec3 lightVec = normalize(globalLight);
    vec3 normalVec = normalize(FragNormal);
    vec3 eyeVec = normalize(FragPosition);
    vec3 refVec = normalize(reflect(eyeVec, normalVec));

    float diffuseVal = max(0.0, dot(normalVec, lightVec));

    float specularVal = pow(max(0.0, dot(refVec, lightVec)), 8.0);

    vec3 finalColor = 0.1 * diffuseColor + diffuseVal * diffuseColor  + specularVal * specularColor;

    OutColor = vec4(finalColor, 1.0);
}
