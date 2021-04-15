
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
out vec4 positionInLightSpace ;
out vec2 texCoord0 ;
out vec3 eye0 ;
	#ifdef FacialTest
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
varying mat3 tbnMat3 ;
varying vec3 normal0 ;
varying vec3 position ;
varying vec4 positionInLightSpace ;
varying vec2 texCoord0 ;
varying vec3 eye0 ;
	#ifdef FacialTest
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
#ifdef FacialTest
	uniform float blendShapeWeight ;
#endif

void main()
{

#ifdef FacialTest
	vec3 pos1 = vertexPosition.xyz*(1.0-blendShapeWeight) ;
	vec3 pos2 = vertexPosition2.xyz*(blendShapeWeight) ;
	position = ( worldMatrix * vec4(pos1 + pos2,1.0) ).xyz ;
#else
	position = ( worldMatrix * vec4(vertexPosition,1.0) ).xyz ;
#endif

    gl_Position = viewProjMatrix * vec4 ( position , 1.0 ) ;
	texCoord0 = vertexTexCoord ;
	eye0 = eye ;
	
	//	caculate normal mapping
	#ifdef FacialTest
		vec3 normal1 = vertexNormal.xyz*(1.0-blendShapeWeight) ;
		vec3 normal2 = vertexNormal2.xyz*(blendShapeWeight) ;
		vec3 n = normalize(( worldMatrix * vec4(normal1 + normal2,1.0) ).xyz) ;		
		
		vec3 tangent1 = vertexTangent.xyz*(1.0-blendShapeWeight) ; 
		vec3 tangent2 = vertexTangent2.xyz*(blendShapeWeight) ;
		vec3 t = normalize(( worldMatrix * vec4(tangent1 + tangent2,1.0) ).xyz) ;
	#else
		vec3 n = normalize(( worldMatrix * vec4(vertexNormal,0.0) ).xyz) ;		
		vec3 t = normalize(( worldMatrix * vec4(vertexTangent,0.0) ).xyz) ;
	#endif
	
	//	orthogonalize
	t = normalize(t-(dot(t,n)*n)) ;
	vec3 biTangent = cross ( t , n ) ;
	tbnMat3 = mat3( t , biTangent , n ) ;
	
	//	orignal normal
	normal0 = n ;
    positionInLightSpace = lightProjMatrix * vec4( position , 1.0 ) ;
	
}
