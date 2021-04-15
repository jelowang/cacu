
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
#endif

#ifdef OPENGLES2
attribute vec3 vertexPosition;
#endif

uniform mat4 viewProjMatrix ;

void main()
{
    gl_Position = viewProjMatrix * vec4 ( vertexPosition , 1.0 ) ;
}
