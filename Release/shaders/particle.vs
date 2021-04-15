
/*

	+	ambient.vs
	+	Jelo Wang
	
	+	hello3d
	+	realtime 3d engine
	
	
	+	(C) Quantum Dynamics Lab.
			Leyoo Co.,Ltd.
	+
 
*/

#ifdef OPENGL3
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;
layout (location = 2) in vec4 vertexColor;
out vec2 texCoord0 ;
out vec4 color0 ;
#endif

#ifdef OPENGLES2
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec4 vertexColor;
varying vec2 texCoord0 ;
varying vec4 color0 ;
#endif

uniform mat4 viewProjMatrix ;

void main()
{
    gl_Position = viewProjMatrix * vec4 ( vertexPosition , 1.0 ) ;
	texCoord0 = vertexTexCoord ;
	color0 = vertexColor ;
}
