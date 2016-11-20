#version 150

// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;
uniform float kick;
uniform float snare;
uniform float hihat;


in vec4 position;
in vec2 texcoord;
in vec4 normal;
in vec4 color;

out vec2 texCoordVarying;


void main()
{
    #ifdef INTEL_CARD
    color = vec4(1.0); // for intel HD cards
    normal = vec4(1.0); // for intel HD cards
    #endif

    texCoordVarying = texcoord;
	//gl_Position = modelViewProjectionMatrix * position;
    
    vec4 modifiedPosition = modelViewProjectionMatrix * position;
    

    //modifiedPosition.y += (snare);
    //modifiedPosition.x += (hihat);
    //modifiedPosition.y += (snare);
    //modifiedPosition.z += kick;
    
    gl_Position = modifiedPosition;
}
