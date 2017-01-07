#version 150

uniform sampler2DRect tex0;
uniform float blurAmnt;
uniform vec4 globalColor;

in vec2 texCoordVarying;
in vec4 positionV;
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
    
    //float r = gl_FragCoord.x / windowWidth;
    //float g = gl_FragCoord.y / windowHeight;
    //float r = 0.0;
    //if (positionV.x > 200) {
    //r = 1.0;
    //}
    
    float min = -200.0;
    float max = 200;
    
    //r = map(positionV.x, -300.0, 300.0, 0.0, 1.0);
    float r = 0.0 + (1.0 - 0.0) * (positionV.x - min) / (max - min);
    
    //float g = 0.0; //positionV.y;
    float g = 0.0 + (1.0 - 0.0) * (positionV.y - min) / (max - min);
    float b = 0.0 + (1.0 - 0.0) * (positionV.z - min) / (max - min);
    float a = 0.1;
    
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
float map(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec2 map(vec2 value, vec2 inMin, vec2 inMax, vec2 outMin, vec2 outMax) {
    return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec3 map(vec3 value, vec3 inMin, vec3 inMax, vec3 outMin, vec3 outMax) {
    return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec4 map(vec4 value, vec4 inMin, vec4 inMax, vec4 outMin, vec4 outMax) {
    return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}
