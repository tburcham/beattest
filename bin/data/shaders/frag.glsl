#version 150 core

//////////////////////////////////////////////////////////////////////
//                                                                  //
//                                                                  //
//  Code developed by Javier Villaroel,                             //
//                  Madrid, Spain, March 2015                       //
//                  javier@ultra-lab.net                            //
//                  www.ultra-lab.net                               //
//                                                                  //
//////////////////////////////////////////////////////////////////////

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2DRect tex0;

//varying vec2 texCoordVarying;
//varying vec2 VTexCoord[];

in vec2 texCoord;
out vec4 outputColor;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;


void main(void)
{
    vec2 center = resolution/2.0;
    
    vec2 pos = texCoord.st;
    float pi = 3.1415;
    float R;
    vec2 r = pos - center;
    float mr = length(r);
    vec2 posM;
    float angT = time; //10.0*time;
    
    float expansionT = 50000000.0;
    float giroT = 20.0;
    float radioMax = 10000.0;
    
    
    
    if (time < expansionT){
        
        R = radioMax - time*(radioMax - 0.5*resolution.y)/expansionT;
    } else {
        if ( time < giroT + expansionT){
            R = resolution.y * 0.5;
            angT =  0.5 * resolution.y * sin(2.0*pi*(time - expansionT)/giroT);
        } else {
            R =  -(radioMax - 0.5*resolution.y)*(giroT+expansionT)/expansionT
            + time*(radioMax - 0.5*resolution.y)/expansionT
            + resolution.y*0.5;
        }
    }
    
    
    if(length(r) < R)
    {
        
        float theta = acos(-r.y/R)/pi;  //angulo con respecto a los polos
        float phi = abs(atan(sqrt(abs(R*R - r.x*r.x - r.y*r.y))/r.x))/pi; //angulo en el plano XY de la esfera
        
        float ajusteP = resolution.y / 2.0 + resolution.x / 2.0 - angT;
        float ajusteN = resolution.x / 2.0 - resolution.y / 2.0 - angT;
        
        if(R <= resolution.y / 2.0)
        {
            posM.y = resolution.y * theta;
            posM.x = resolution.y * phi;
        }
        
        if(R > resolution.y / 2.0 )
        {
            float alpha = acos(resolution.y/(2.0*R))/pi;
            float A = resolution.y /(1.0-(2.0*alpha));
            
            float beta = acos(resolution.y/(2.0*R))/pi;
            float B = resolution.y /(1.0-(2.0*beta));
            
            posM.y = A * (theta - alpha);
            posM.x = B * (phi - beta);
        }
        
        if(r.x > 0.0){
            posM.x =  ajusteP - posM.x;
        } else {
            posM.x += ajusteN;
        }
    
        
        posM.x /= 10;
        posM.y /= 10;
        
        if (posM.x <= 0.0 || posM.y <= 0.0 || posM.y > resolution.y || posM.x > resolution.x){
            /*if (resolution.x > 0) {
             
             outputColor = vec4(1.0, 0.0, 0.0, 1.0);
             } else {
             
             discard;
             
             }*/
            
            discard;
        } else {
            vec4 color = texture(tex0, posM);
            color.a = 125;
            outputColor = color;
            //gl_FragColor = gl_Color;
        }
        
        
    } else {
        
        discard;
        
    }
    
    
}

void __main(void)
{
    
    vec4 color = texture(tex0,vec2(100,0));
    outputColor = color;
}
