
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
in vec4 positionInLightSpace ;
in mat3 tbnMat3 ;
in vec3 normal0 ;
in vec2 texCoord0 ;
in vec3 eye0 ;
#endif

#ifdef OPENGLES2
varying vec3 position ;
varying vec4 positionInLightSpace ;
varying vec3 normal0 ;
varying mat3 tbnMat3 ;
varying vec2 texCoord0 ;
varying vec3 eye0 ;
#endif

struct DirectionalLight {
	vec3 color ;
	vec3 direction ;
	float intensity ;
} ;

struct Material {
	float sss ;
    float shininess ;
	float specularIntensity ;
	float fresnel ;
    vec3 ambientColor ;
	vec3 diffuseColor ;
	vec3 specularColor ;
} ; 

uniform Material material ;
uniform DirectionalLight dlight ;
uniform sampler2D diffuseTexture ;
uniform sampler2D normalTexture ;
uniform sampler2D shadowTexture ;

#ifdef OPENGL3
float shadowCalculation(vec4 fragPosLightSpace,vec3 lightDir,vec3 normal)
{
	
   // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(shadowTexture, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float bias = 0.005 ;
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
	float shadow = 0.0;
	float texsizex = float(textureSize(shadowTexture, 0).x);
	float texsizey = float(textureSize(shadowTexture, 0).y); 
    vec2 texelSize = 1.0 / vec2(texsizex,texsizey) ;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture2D(shadowTexture, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
	  
    return shadow;
}
#endif

vec4 calcLightPongModel ( vec3 lightColor , vec3 directionLightColor , vec3 lightDir , vec3 normal , float intensity )
{

	vec4 diffuseColor = vec4(0) ;
	vec4 specularColor = vec4(0) ;
	
	// Diffuse 
    float diff = max(dot(normal, -lightDir), 0.0);
	if ( diff > 0.0 ) {
		diffuseColor = vec4(lightColor,1.0) * intensity * diff ;
	}
	
	vec3 viewDir = normalize(eye0 - position);
	vec3 reflectDir = normalize(reflect(lightDir,normal));  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	if ( spec > 0.0 ) {
		vec3 color = directionLightColor * spec * intensity ;  	
		specularColor = vec4(color,1.0) ;  
	}
	return diffuseColor + specularColor ;
	  
}

#if 0
void main ()
{
	
	vec3 normal = normal0 ;
	normal = normalize(tbnMat3 * (255.0/128.0 * texture2D(normalTexture,texCoord0.xy).xyz - 1.0)) ;
	  
#ifdef OPENGL3		
	//float shadowContributions = shadowCalculation(positionInLightSpace,dlight.direction , normal) ;                      
	// reduce shadow strength a little: allow some diffuse/specular light in shadowed regions 
    //shadowContributions = min(shadowContributions, 0.75); 
	
	//vec4 shadowDiffuseSpecularContributions = calcLightPongModel( material.ambientColor , dlight.color , dlight.direction , normal , dlight.intensity ) * (1.0 - shadowContributions) ;
#else
	//vec4 shadowDiffuseSpecularContributions = calcLightPongModel( material.ambientColor , dlight.color , dlight.direction , normal , dlight.intensity ) ;
#endif	  
	
	vec4 pongColor = calcLightPongModel( material.ambientColor , dlight.color , dlight.direction , normal , dlight.intensity ) ; 
	vec3 textureColor = (texture2D ( diffuseTexture , texCoord0.xy ).a * texture2D(diffuseTexture,texCoord0.xy).rgb).rgb * pongColor.xyz ;
	fragColor = vec4(textureColor,1.0) ;
	
	//float depth = texture2D(diffuseTexture,texCoord0.xy).r ;
	//fragColor = vec4(vec3(depth),1.0) ;  
}
#endif

#if 1
float saturate( float v)
{
    if( v < 0.0 ) v = 0.0;
    if( v > 1.0 ) v = 1.0;
	return v ; 
}

float Rs(float m,float F,vec3 N, vec3 L,vec3 V, vec3 H)
{
    float result;
    float NdotV= dot(N,V);
    float NdotH= dot(N,H);
    float NdotL= dot(N,L);
    float VdotH= dot(V,H);
    float Geom= min(min(1.0, (2.0*NdotV*NdotH)/VdotH), (2.0*NdotL*NdotH)/VdotH);
    float Rough= pow(1.0/(pow(m,2.0)*pow(NdotH,4.0)), ( pow(NdotH,2.0)-1.0)/( pow(m,2.0)*pow(NdotH,2.0)));
    float Fresnel= F + pow(1.0-VdotH,5.0) * (1.0-F);
    return (Fresnel * Rough * Geom)/(NdotV*NdotL);
}

float G1V(float dotNV , float k )
{
	return 1.0/(dotNV*(1.0-k)+k) ;
}		 

float lightingFuncGGX_REF(vec3 N , vec3 V , vec3 L , float roughness , float F0)
{
	float alpha = roughness*roughness ;
	
	vec3 H = normalize(V+L) ;
	
	float dotNL = saturate(dot(N,L)) ;
	float dotNV = saturate(dot(N,V)) ;
	float dotNH = saturate(dot(N,H)) ;
	float dotLH = saturate(dot(L,H)) ;

	float F , D , vis ;
	
	//D 
	float alphaSqr = alpha*alpha ;
	float pi = 3.14159 ;
	float denom = dotNH*dotNH*(alphaSqr-1.0) + 1.0 ;
	D = alphaSqr/(pi*denom*denom) ;
	   
	//F
	float dotLH5 = pow(1.0-dotLH,5.0) ;
	F = F0 + (1.0-F0)*(dotLH5) ;
	
	//V
	float k = alpha/2.0 ;
	vis = G1V(dotNL,k)*G1V(dotNV,k) ;
	
	float specular = dotNL*D*F*vis;
	return specular ;
	
}

#define PI 3.1415926535
float chiGGX(float v)
{
    if ( v > 0.0 ) 
		return 1.0 ;
	return 0.0; 
}		 
float GGX_Distribution(vec3 n, vec3 h, float alpha)
{
    float NoH = dot(n,h);
    float alpha2 = alpha * alpha;
    float NoH2 = NoH * NoH;
    float den = NoH2 * alpha2 + (1.0 - NoH2);
    return (chiGGX(NoH) * alpha2) / ( PI * den * den );
}

float GGX_PartialGeometryTerm(vec3 v, vec3 n, vec3 h, float alpha)
{
    float VoH2 = saturate(dot(v,h));
    float chi = chiGGX( VoH2 / saturate(dot(v,n)) );
    VoH2 = VoH2 * VoH2;
    float tan2 = ( 1.0 - VoH2 ) / VoH2;
    return (chi * 2.0) / ( 1.0 + sqrt( 1.0 + alpha * alpha * tan2 ) );
}

vec3 Fresnel_Schlick(float cosT, vec3 F0)
{
  return F0 + (1.0-F0) * pow( 1.0 - cosT, 5.0);
}

float _SP = 1.4 ;
vec4 LightingCookTorranceTest(vec3 texColor,DirectionalLight base,vec3 normal, vec3 lightDir, vec3 viewDir, float atten)  
{  

	if ( viewDir.x != 0.0 || viewDir.y != 0.0 || viewDir.z != 0.0 ) {
		viewDir = normalize(viewDir) ;
	}
	
	//viewDir.x = abs(viewDir.x) ;
	//viewDir.y = abs(viewDir.y) ;
	//viewDir.z = abs(viewDir.z) ;
	 
	vec3 l = normalize(-lightDir);  
	vec3 v = viewDir;  
	vec3 n = normalize(normal);  
	
	vec3 h = normalize(l+v); 
			
	float NdotH = dot(n,h) ;
	float NdotV = dot(n,v) ;
	float NdotL = dot(n,l) ;
	float VdotH = dot(v,h) ;
	
	//NDF D = which surface points such that they could reflect light form l to v
   //float D = ( ((material.sss + 2.0) pow(dot(n, h) , material.sss )) / 4.0*3.1415926);   
	float D = (material.sss + 2.0) / 8.0 * pow(dot(n, h),material.sss) / 4.0*3.1415926 ; 
	 
	vec3 FresnelColor = vec3(material.fresnel,material.fresnel,material.fresnel) ;
	//The Fresnel relfectance function computes the fraction of light reflected form an optically flat surface.
	vec3 F = (FresnelColor + (1.0-FresnelColor)*pow((1.0-dot(h,l)), 5.0)); 	
	
	//the percentage of surface points with m = h that are not shadowed or masked with l and v
	//G
	float coffK = 2.0 / sqrt( 3.1415926*(material.sss+2.0) ) ;
	float coffV = 1.0 / ( (NdotL*(1.0-coffK)+coffK )*(NdotV*(1.0-coffK)+coffK) ) ;
	float all = D*F.r*coffV ; 
	float diffuse = dot(n,l) ;
	diffuse = (1.0-all) * diffuse ;
	  
	//vec4 color = vec4( base.color*texColor*GGX_Distribution(n,v,material.sss)*texColor,1.0) ;
	
	vec4 color = vec4( texColor*(lightingFuncGGX_REF(n,v,l,material.shininess,F.r) * base.color*base.intensity ).rgb ,1.0) ;  
	  
   return diffuse * color * atten ;  

}  

		
void main()
{
	vec3 normal = normal0 ;
	normal = normalize(tbnMat3 * (255.0/128.0 * texture2D(normalTexture,texCoord0.xy).xyz - 1.0)) ;
	
#ifdef OPENGL3		
	//float shadowContributions = shadowCalculation(positionInLightSpace,dlight.direction , normal) ;                      
	// reduce shadow strength a little: allow some diffuse/specular light in shadowed regions 
    //shadowContributions = min(shadowContributions, 0.75); 
	
	//vec4 shadowDiffuseSpecularContributions = calcLightPongModel( material.ambientColor , dlight.direction , normal , dlight.intensity ) * (1.0 - shadowContributions) ;
#else
	//vec4 shadowDiffuseSpecularContributions = calcLightPongModel( material.ambientColor , dlight.direction , normal , dlight.intensity ) ;
#endif	

	//fragColor = texture2D(diffuseTexture,texCoord0.xy) * shadowDiffuseSpecularContributions ;
	normal = normal0 ;  
	fragColor = LightingCookTorranceTest( (texture2D ( diffuseTexture , texCoord0.xy ).a * texture2D(diffuseTexture,texCoord0.xy).rgb ),dlight,normalize(normal),dlight.direction , eye0 , material.specularIntensity) ;	


}
#endif