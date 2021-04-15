
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
in vec4 color0 ;
#endif

#ifdef OPENGLES2
varying vec2 texCoord0 ;
varying vec4 color0 ;
#endif

uniform sampler2D diffuseTexture ;

void main ()
{
	//fragColor = vec4(color0.r*(color0.xyz*texture2D(diffuseTexture,texCoord0.xy).xyz),1.0) ;
	fragColor = vec4( (texture2D(diffuseTexture,texCoord0.xy).xzy * color0.xyz) * color0.w ,1.0) ;
	//fragColor = vec4(vec3(color0.w,color0.w,color0.w),1.0) ;
}

