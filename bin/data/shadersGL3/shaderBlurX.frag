#version 150

uniform sampler2DRect tex0;
uniform float blurAmnt;
uniform vec4 globalColor;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{

    vec4 color;
	
    
    //color = sphereColor;
    

        // gl_FragCoord contains the window relative coordinate for the fragment.
        // we use gl_FragCoord.x position to control the red color value.
        // we use gl_FragCoord.y position to control the green color value.
        // please note that all r, g, b, a values are between 0 and 1.
        
        float windowWidth = 2048.0;
        float windowHeight = 1536.0;
        
        float r = gl_FragCoord.x / windowWidth;
        float g = gl_FragCoord.y / windowHeight;
        float b = 1.0;
        float a = 0.25;
    
        color = vec4(r, g, b, a);
    //color += 1.0;
    
    /*color += 1.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -4.0, 0.0));
    color += 2.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -3.0, 0.0));
    color += 3.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -2.0, 0.0));
    color += 4.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -1.0, 0.0));
    
    color += 5.0 * texture(tex0, texCoordVarying + vec2(blurAmnt, 0));
    
    color += 4.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 1.0, 0.0));
    color += 3.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 2.0, 0.0));
    color += 2.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 3.0, 0.0));
    color += 1.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 4.0, 0.0));
    
    color /= 25.0;*/

    
    outputColor = color;

    
   // outputColor = globalColor;
}
