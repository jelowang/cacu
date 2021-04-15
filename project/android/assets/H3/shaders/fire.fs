
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







void main ()
{

	resolution = vec2 (1024,1024) ;
	mouse = vec2 (0.0,0.0) ;	
	
	vec2 pos = ( texCoord0.xy  )*8.-vec2(4.,5.);// + mouse / 4.0;
	pos.x = (pos.x/1.420);
	if(pos.y>-2.*4.20){
		for(float baud = 1.; baud < 9.; baud += 1.){
			pos.y += 0.2*sin(4.20*time/(1.+baud))/(1.+baud);
			pos.x += 0.1*cos(pos.y/4.20+2.40*time/(1.+baud))/(1.+baud);
		}
		//0.13*cos(time*2.+0.6)+.1*sin(time*3.+0.4)+
		pos.y += 0.04*fract(sin(time*60.));
	}

	pos.x = abs(pos.x); //left side was missing
	vec3 color = vec3(0.,0.,0.0);
	
	float p =.004;
	float y = -pow(pos.x,4.20)/(4.20*p)*4.20;
	float dir = length(pos-vec2(pos.x,y))*sin(0.3);//*(0.01*sin(time)+0.07);
	if(dir < 0.7){
		color.rg += smoothstep(0.0,1.,.75-dir);
		//color.g /=2.0;
	}
	color *= (0.2+abs((pos.y/4.2+4.2))/4.2);
	color += pow(color.r,1.1);
	color *= cos(-0.5+pos.y*0.4);
	
	pos.y += 1.5;
	vec3 dolor = vec3(0.,0.,0.0);
	y = -pow(pos.x,4.20)/(4.20*p)*4.20;
	dir = length(pos-vec2(pos.x,y))*sin(0.3);
	if(dir < 0.7){
		dolor.bg += smoothstep(0.0,1.,.75-dir);
		dolor.g /=2.4;
	}
	dolor *= (0.2+abs((pos.y/4.2+4.2))/4.2);
	dolor += pow(color.b,1.1);
	dolor *= cos(-0.6+pos.y*0.4);
	//dolor.rgb -= pow(length(dolor)/16., 0.5);
	color = (color+dolor)/2.;
	fragColor = vec4(vec3(color) , 1.0 );

	//fragColor = vec4( texture2D ( diffuseTexture , texCoord0.xy ).rgb * ambient.color , 1.0 );	
	
	//float depth = texture2D(diffuseTexture,texCoord0.xy).r ;
	//fragColor = vec4(vec3(depth),1.0) ;
	
}
