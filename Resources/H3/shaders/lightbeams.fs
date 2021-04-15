
/*

	+	ambient.vs
	+	Jelo Wang
	
	+	hello3d
	+	realtime 3d engine
	
	
	+	(C) Quantum Dynamics Lab.
			Leyoo Co.,Ltd.
	+
 
*/

#ifdef OPENGLES2
precision mediump float ;
#define texture2D texture2D
#define fragColor gl_FragColor
#endif

#ifdef OPENGL3
#define texture2D texture
#endif

#ifdef OPENGL3
layout (location = 0) out vec4 fragColor;
in vec2 texCoord0 ;
#endif

#ifdef OPENGLES2
varying vec2 texCoord0 ;
#endif

struct Ambient {
    vec3 color ;
} ; 

struct Material {
    float shininess ;
	float specularIntensity ;
    vec3 ambientColor ;
	vec3 diffuseColor ;
	vec3 specularColor ;
} ; 

uniform Material material ;
uniform Ambient ambient ;
uniform sampler2D diffuseTexture ;

uniform float time ;
vec2 mouse;
vec2 resolution;



float hash( float n ) { return fract(sin(n)*753.5453123); }

// Slight modification of iq's noise function.
float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    
    float n = p.x + p.y*157.0;
    return mix(
                    mix( hash(n+  0.0), hash(n+  1.0),f.x),
                    mix( hash(n+157.0), hash(n+158.0),f.x),
            f.y);
}


float fbm(vec2 p, vec3 a)
{
     float v = 0.0;
     v += noise(p*a.x)*.5;
     v += noise(p*a.y)*.25;
     v += noise(p*a.z)*.125;
     return v;
}

vec3 drawLines( vec2 uv, vec3 fbmOffset, vec3 color1, vec3 color2 )
{
    float timeVal = time * 0.1;
    vec3 finalColor = vec3( 0.0 );
    for( int i=0; i < 3; ++i )
    {
        float indexAsFloat = float(i);
        float amp = 40.0 + (indexAsFloat*5.0);
        float period = 2.0 + (indexAsFloat+2.0);
        float thickness = mix( 0.9, 1.0, noise(uv*10.0) );
        float t = abs( 0.9 / (sin(uv.x + fbm( uv + timeVal * period, fbmOffset )) * amp) * thickness );
        
        finalColor +=  t * color1;
    }
    
    for( int i=0; i < 5; ++i )
    {
        float indexAsFloat = float(i);
        float amp = 40.0 + (indexAsFloat*7.0);
        float period = 2.0 + (indexAsFloat+8.0);
        float thickness = mix( 0.7, 1.0, noise(uv*10.0) );
        float t = abs( 0.8 / (sin(uv.x + fbm( uv + timeVal * period, fbmOffset )) * amp) * thickness );
        
        finalColor +=  t * color2 * 0.6;
    }
    
    return finalColor;
}



void main ()
{

	resolution = vec2 (1024,1024) ;
	mouse = vec2 (0.0,0.0) ;	
	
    vec2 uv = ( texCoord0.xy  ) * 2.0 - 1.0;
    uv.x *= resolution.x/resolution.y;
    uv.xy = uv.yx;

    vec3 lineColor1 = vec3( 0.3, 0.5, .5 );
    vec3 lineColor2 = vec3( 0.3, 0.5, 2.5 );
    
    vec3 finalColor = vec3(0.0);

    
    float t = sin( time ) * 0.5 + 0.5;
    float pulse = mix( 0.10, 0.20, t);
    
    finalColor += drawLines( uv, vec3( 1.0, 20.0, 30.0), lineColor1, lineColor2 ) * pulse;
    finalColor += drawLines( uv, vec3( 1.0, 2.0, 4.0), lineColor1, lineColor2 );
	
	if ((mouse.x  * 2. - 1.) * resolution.x/resolution.y > uv.y) {
		finalColor.rgb = finalColor.bgr;
	}
    
    fragColor = vec4( finalColor, 1.0 );

	//fragColor = vec4( texture2D ( diffuseTexture , texCoord0.xy ).rgb * ambient.color , 1.0 );	
	
	//float depth = texture2D(diffuseTexture,texCoord0.xy).r ;
	//fragColor = vec4(vec3(depth),1.0) ;
	
}
