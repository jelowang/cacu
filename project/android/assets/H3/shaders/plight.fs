
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
in vec3 position ;
in mat3 tbnMat3 ;
in vec3 normal0 ;
in vec2 texCoord0 ;
in vec3 eye0 ;
#endif

#ifdef OPENGLES2
varying vec3 position ;
varying vec3 normal0 ;
varying mat3 tbnMat3 ;
varying vec2 texCoord0 ;
varying vec3 eye0 ;
#endif

struct PointLight {
	vec3 position ;  
	vec3 color ;  
    float constant ;
    float linear ;
    float quadratic ;
	float intensity ;
	float range ;
}; 

struct Material {
    float shininess ;
	float specularIntensity ;
    vec3 ambientColor ;
	vec3 diffuseColor ;
	vec3 specularColor ;
} ; 

uniform Material material ;
uniform PointLight plight ;
uniform sampler2D diffuseTexture ;
uniform sampler2D normalTexture ;

vec4 calcLightPongModel ( vec3 lightColor , vec3 lightDir , vec3 normal , float intensity )
{

	vec4 diffuseColor = vec4(0.0) ;
	vec4 specularColor = vec4(0.0) ;
	
	// Diffuse 
    float diff = max(dot(normal, -lightDir), 0.0);
	if ( diff > 0.0 ) {
		diffuseColor = vec4(lightColor,1.0) * intensity * diff ;
	}
	
	vec3 viewDir = normalize(eye0 - position);
	vec3 reflectDir = normalize(reflect(lightDir,normal));  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	if ( spec > 0.0 ) {
		specularColor = vec4(lightColor,1.0) * spec * material.specularIntensity ;  	
	}
	
	return diffuseColor + specularColor ;
	
}

void main ()
{

	vec3 normal = normal0 ;
	normal = normalize(tbnMat3 * (255.0/128.0 * texture2D(normalTexture,texCoord0.xy).xyz - 1.0)) ;
	
	float distance = length(plight.position - position);
	float attenuation = 1.0 / (plight.constant + plight.linear * distance + plight.quadratic * (distance * distance)); 
	
	vec3 plightDir = plight.position - position ;

	fragColor = texture2D(diffuseTexture,texCoord0.xy) * calcLightPongModel ( plight.color , plightDir , normal , plight.intensity ) * plight.intensity * attenuation ;	

}

