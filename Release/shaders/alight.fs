
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
	float sss ;
    float shininess ;
	float specularIntensity ;
	vec3 fresnel ;
    vec3 ambientColor ;
	vec3 diffuseColor ;
	vec3 specularColor ;
} ; 

uniform Material material ;
uniform Ambient ambient ;
uniform sampler2D diffuseTexture ;

void main ()
{
	fragColor = vec4( texture2D ( diffuseTexture , texCoord0.xy ).a * texture2D ( diffuseTexture , texCoord0.xy ).rgb * material.ambientColor * ambient.color , texture2D ( diffuseTexture , texCoord0.xy ).a );	
}
