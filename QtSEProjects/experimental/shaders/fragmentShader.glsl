#version 430

precision highp float;

uniform sampler2D tex;

in vec2 fragUV;
in vec4 fragColor;

out vec4 outColor;

void main()
{
	//outColor = vec4( fragUV , 1.0 , 1.0 );
	//outColor = vec4( 0.0 , 1.0 , 1.0 , 1.0 );
	//outColor = vec4( fragColor , 1.0 );
	outColor = texture( tex , fragUV ).rgba;// * vec4( fragColor , 1.0 );
	//outColor = vec4( texture( tex , fragUV ).rgb , 1.0 );// * vec4( fragColor , 1.0 );
	//outColor = texture( tex , vec2( 0.0 , 0.0 ) ).rgb * fragColor;
}
