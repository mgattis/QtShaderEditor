#version 440 core
out vec4 OutColor;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec2 Resolution;
uniform float Time;

uniform sampler2D AmbientMap;
uniform sampler2D DiffuseMap;

uniform sampler2D SponzaFramebuffer;

smooth in vec2 FragTexCoord;

vec3 dither8Color(vec2 q, vec3 inColor) {
    mat4 d = mat4(
        1.0, 13.0, 4.0, 16.0,
        9.0, 5.0, 12.0, 8.0,
        3.0, 15.0, 2.0, 14.0,
        11.0, 7.0, 10.0, 6.0) / 17.0;

    q = floor(q);
    q = mod(q, 4.0);
    float value = d[int(q.x)][int(q.y)];

    vec3 outColor = vec3(0.0, 0.0, 0.0);
    if (inColor.r > value) { outColor.r = 1.0; }
    if (inColor.g > value) { outColor.g = 1.0; }
    if (inColor.b > value) { outColor.b = 1.0; }

    return outColor;
}

void main() {
    vec2 q = FragTexCoord * Resolution;

    vec3 color = texture(SponzaFramebuffer, FragTexCoord.xy).xyz;

    //color = dither8Color(q, color);

    OutColor = vec4(color, 1.0);
}
