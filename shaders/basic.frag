#version 330 core

out vec4 FragColor;
in vec2 texCoord;

//Alpha cutoff not to render
float cutoff = 1.0;

uniform sampler2D outTexture;

void main()
{
	
	FragColor = texture(outTexture, texCoord);
	
	if(FragColor.a < cutoff){
		discard;
	}
}