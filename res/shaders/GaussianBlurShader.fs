#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler;
uniform vec4 lightDirection;

out vec4 fragColor;
void main()
{
	//fragColor = texture(sampler, texCoord0)* vec4(color0,1.0); //you must have gl_FragColor

	vec4 blurCon= vec4(0.0,0.0,0.0,1.0); //define new base frag
        float kernel[9] = float[9](
        1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
        2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
        1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0);
    for(int y=0;y<3;y++){
        for(int x=0;x<3;x++){
            ivec2 offset = ivec2(x-1,y-1); //get current offset compared to central frag
            vec2 curTexCoord = texCoord0+(offset);
            blurCon += texture(sampler, curTexCoord) * kernel[y*3 + x];
        }
    }
    fragColor=blurCon;
    fragColor.a=1.0; //just in case it was changed, opacity set to none
}