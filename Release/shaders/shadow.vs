
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
layout (location = 1) in vec2 vertexTexCoord ;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec3 vertexTangent;
out mat3 tbnMat3 ;
out vec3 normal0 ; 
out vec3 position ;
out vec2 texCoord0 ;
out vec3 eye0 ;
#endif

#ifdef OPENGLES2
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord ;
attribute vec3 vertexNormal;
attribute vec3 vertexTangent;
varying mat3 tbnMat3 ;
varying vec3 normal0 ;
varying vec3 position ;
varying vec2 texCoord0 ;
varying vec3 eye0 ;
#endif

uniform mat4 worldMatrix ;
uniform mat4 viewProjMatrix ;
uniform mat4 lightProjMatrix ;
uniform vec3 eye ;

void main()
{
	position = ( worldMatrix * vec4(vertexPosition,1.0) ).xyz ;	
    gl_Position = lightProjMatrix * vec4 ( position , 1.0 ) ;
}
