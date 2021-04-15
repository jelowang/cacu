
/*

	+	ambient.vs
	+	Jelo Wang
	
	+	hello3d
	+	realtime 3d engine
	
	
	+	(C) Quantum Dynamics Lab.
			Leyoo Co.,Ltd.
	+
 
*/

#define FACIAL
#ifdef OPENGL3
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord ;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec3 vertexTangent;
out vec2 texCoord0 ;
	#ifdef FACIAL
		layout (location = 4) in vec3 vertexPosition2;
		layout (location = 5) in vec2 vertexTexCoord2 ;
		layout (location = 6) in vec3 vertexNormal2;
		layout (location = 7) in vec3 vertexTangent2;
	#endif
#endif

#ifdef OPENGLES2
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord ;
attribute vec3 vertexNormal;
attribute vec3 vertexTangent;
varying vec2 texCoord0 ;
	#ifdef FACIAL
		attribute vec3 vertexPosition2;
		attribute vec2 vertexTexCoord2 ;
		attribute vec3 vertexNormal2;
		attribute vec3 vertexTangent2;
	#endif
#endif

uniform mat4 worldMatrix ;
uniform mat4 viewProjMatrix ;
uniform mat4 lightProjMatrix;
uniform vec3 eye ;

#ifdef FACIAL
	uniform float blendShapeWeight ;
#endif
   
void main()
{
#ifdef FACIAL
	vec3 pos1 = vertexPosition.xyz*(1.0-blendShapeWeight) ;
	vec3 pos2 = vertexPosition2.xyz*(blendShapeWeight) ;
	vec3 position = ( worldMatrix * vec4(pos1 + pos2,1.0) ).xyz ;
#else
	vec3 position = ( worldMatrix * vec4(vertexPosition,1.0) ).xyz ;
#endif

    gl_Position = viewProjMatrix * vec4 ( position , 1.0 ) ; 
	texCoord0 = vertexTexCoord ;

}
