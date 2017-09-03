/***********************************************
 Geometry shader to convert lines into triangle strips
 Memo Akten
 www.memo.tv

************************************************/

#version 120
#extension GL_EXT_geometry_shader4 : enable

uniform float thickness;
uniform vec3 lightDir;
//varying vec2 glTexCoord;
//varying vec2 texCoordVarying;

uniform sampler2DRect tex0;

float rand(vec3 co, float seed){
   // return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,45.123))) * 43758.5453);
    return fract( sin( dot( co.xyz + seed, vec3(12.9898,78.233,45.123) ) ) * 43758.5453 + seed ) ;

}
float random( vec2 p )
{
    // We need irrationals for pseudo randomness.
    // Most (all?) known transcendental numbers will (generally) work.
    const vec2 r = vec2(
                        23.1406926327792690,  // e^pi (Gelfond's constant)
                        2.6651441426902251); // 2^sqrt(2) (Gelfond–Schneider constant)
    return fract( cos( mod( 123456789., 1e-7 + 256. * dot(p,r) ) ) );
}


void main() {
    
    //texCoordVarying = glTexCoord[0].xy;
    
	vec3 p0 = gl_PositionIn[0].xyz;
	vec3 p1 = gl_PositionIn[1].xyz;
    
    //p0 = rand(p0);
    //p1 = rand(p1);

    
    float r = random( p0.xy );
    //p0 *= r;
    
    float r1 = random( p1.xy );
    p1 *= r1;
    
    float t1 = random( p0.xy );
    
    //vec2 pos = gl_TexCoordIn.st;
    //vec4 color = texture2DRect(tex0, pos);
	
	vec3 up = vec3(0, 0, 1);	// arbitrary up vector
	
	vec3 dir = normalize(p1 - p0);			// normalized direction vector from p0 to p1
	vec3 right = normalize(cross(dir, up));	// right vector
	vec3 norm = cross(right, dir);
	float fColMult = abs(dot(norm, lightDir));
	vec4 colMult = vec4(fColMult, fColMult, fColMult, 3.0);
	
	right *= (thickness * (2 * t1));
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(p0 - right, 1.0);
	gl_FrontColor = gl_FrontColorIn[0] * colMult;
    //gl_FrontColor = color * colMult;
	EmitVertex();
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(p0 + right, 1.0);
	gl_FrontColor = gl_FrontColorIn[0] * colMult;
    //gl_FrontColor = color * colMult;
	EmitVertex();
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(p1 - right, 1.0);
	gl_FrontColor = gl_FrontColorIn[1] * colMult;
    //gl_FrontColor = color * colMult;
	EmitVertex();

	gl_Position = gl_ModelViewProjectionMatrix * vec4(p1 + right, 1.0);
	gl_FrontColor = gl_FrontColorIn[1] * colMult;
    //gl_FrontColor = color * colMult;
	EmitVertex();

}


