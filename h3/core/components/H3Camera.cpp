
/*

	+	H3Camera.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Math.h"
#include "H3Camera.h"
#include "H3Transform.h"

/*
 brief :
*/
H3Camera::H3Camera () 
{

	this->eular.setBank ( -1.0f ) ;
	this->eular.setHead ( 1.0f ) ;
	this->eular.setPitch ( 1.0f ) ;

}

/*
 brief :
*/
H3Camera::~H3Camera () 
{
}

/*
 brief :
*/
H3Matrix4x4f H3Camera::getProjection () 
{
	return this->projection ;
}

/*
 brief : projMatrix mul cameraMatrix
*/
H3Matrix4x4f H3Camera::getViewProjection () 
{
	H3Matrix4x4f results ;	
	H3Quaternion rotate = this->h3Transform.getRotate ().conjugate () ;
	H3Matrix4x4f rotateMatrix = rotate.getMatrixForm () ;
	//	reverse camera direction
	//	because at ModelView Matrix , an object is moving to backward (add nagetive value) when camera goes forward
	H3Vector3f position = this->h3Transform.getPosition ().mul ( -1.0f ) ;
	H3Matrix4x4f translation ;
	
	translation.initTranslate ( position ) ;

	results = this->projection * rotateMatrix * translation ;

	return results ;
}

/*
 brief : 
*/
H3Matrix4x4f H3Camera::getView () 
{
	H3Matrix4x4f results ;	
	H3Quaternion rotate = this->h3Transform.getRotate ().conjugate () ;
	H3Matrix4x4f rotateMatrix = rotate.getMatrixForm () ;
	//	reverse camera direction
	//	because at ModelView Matrix , an object is moving to backward (add nagetive value) when camera goes forward
	H3Vector3f position = this->h3Transform.getPosition ().mul ( -1.0f ) ;
	H3Matrix4x4f translation ;
	
	translation.initTranslate ( position ) ;

	results = rotateMatrix * translation ;

	return results ;
}

/*
  brief : ortho
*/
extern int default_width ;
extern int default_height ;
void H3Camera::initOrtho ( iF32 left , iF32 right , iF32 top , iF32 bottom , iF32 zNear , iF32 zFar )
{
    
    H3Matrix4x4f* m = &this->projection ;
    
    iF32 m00 = 2.0 / (right - left) ;
    iF32 m11 = 2.0 / (top - bottom) ;
    iF32 m22 = -2.0 / (zFar - zNear) ;
    
    iF32 m03 = -(right + left) / (right - left) ;
    iF32 m13 = -(top + bottom) / (top - bottom) ;
    iF32 m23 = -(zFar + zNear) / (zFar - zNear) ;
    
    m->m[0][0] = m00 ;  m->m[0][1] = 0 ;    m->m[0][2] = 0 ;	m->m[0][3] = m03 ;
    m->m[1][0] = 0 ;    m->m[1][1] = m11 ;	m->m[1][2] = 0 ;	m->m[1][3] = m13 ;
    m->m[2][0] = 0 ;    m->m[2][1] = 0 ;	m->m[2][2] = m22 ;	m->m[2][3] = m23 ;
    m->m[3][0] = 0 ;    m->m[3][1] = 0 ;	m->m[3][2] = 0 ;	m->m[3][3] = 1.0 ;

    //m->m[0][0] = 2.0f/default_width ;	m->m[0][1] = 0 ;						m->m[0][2] = 0 ;	m->m[0][3] = -1.0 ;
	//m->m[1][0] = 0 ;					m->m[1][1] = 2.0f/-default_height ;		m->m[1][2] = 0 ;	m->m[1][3] = 1.0 ;
    //m->m[2][0] = 0 ;					m->m[2][1] = 0 ;						m->m[2][2] = -1.0 ;	m->m[2][3] = 0.0 ;
    //m->m[3][0] = 0 ;					m->m[3][1] = 0 ;						m->m[3][2] = 0 ;	m->m[3][3] = 1.0 ;

	this->type = H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC;
}

/*
 brief : right-hands
 */
void H3Camera::initProjection ( iF32 fov , iF32 aspectRatio , iF32 zNear , iF32 zFar ) 
{
 
   
    fov = H3ToRadian ( fov ) ;
    
	iF32 tanHalfFOV = (iF32 )tanf((fov / 2.0f)) ;
	iF32 zRange = zNear - zFar ;
    
    iF32 m00 = 1.0f / (tanHalfFOV*aspectRatio) ;
    iF32 m11 = 1.0f / tanHalfFOV ;
    iF32 m22 = (-zNear - zFar)/zRange ;
    iF32 m23 = 2.0f * zFar * zNear / zRange ;
    
	H3Matrix4x4f* m = &this->projection ;

	m->m[0][0] = m00 ;	m->m[0][1] = 0 ;	m->m[0][2] = 0 ;    m->m[0][3] = 0 ;
	m->m[1][0] = 0 ;    m->m[1][1] = m11 ;  m->m[1][2] = 0 ;	m->m[1][3] = 0 ;
	m->m[2][0] = 0 ;    m->m[2][1] = 0 ;	m->m[2][2] = m22 ;	m->m[2][3] = m23 ;
	m->m[3][0] = 0 ;    m->m[3][1] = 0 ;	m->m[3][2] = 1 ;	m->m[3][3] = 0 ;
    
	this->type = H3COMPONENT_TYPE_CAMERA_PERSPECTIVE;

}

/*
 brief : left-hands
 */
void H3Camera::initProjection ( iF32 left , iF32 right , iF32 top , iF32 bottom , iF32 zNear , iF32 zFar )
{
    
    H3Matrix4x4f* m = &this->projection ;
    
    iF32 m00 = (2.0 * zNear) / (right - left) ;
    iF32 m11 = (2.0 * zNear) / (top - bottom) ;
    
    iF32 m02 = (right + left) / (right - left) ;
    iF32 m12 = (top + bottom) / (top - bottom) ;
    iF32 m22 = -(zFar + zNear) / (zFar - zNear) ;
    
    iF32 m23 = -(2.0 * zFar * zNear) / (zFar - zNear) ;
    
    m->m[0][0] = m00 ;  m->m[0][1] = 0 ;    m->m[0][2] = m02 ;	m->m[0][3] = 0 ;
    m->m[1][0] = 0 ;    m->m[1][1] = m11 ;	m->m[1][2] = m12 ;	m->m[1][3] = 0 ;
    m->m[2][0] = 0 ;    m->m[2][1] = 0 ;	m->m[2][2] = m22 ;	m->m[2][3] = m23 ;
    m->m[3][0] = 0 ;    m->m[3][1] = 0 ;	m->m[3][2] = -1.0 ;	m->m[3][3] = 0 ;
    
	this->type = H3COMPONENT_TYPE_CAMERA_PERSPECTIVE;
}

/*
 brief : check is perspective projection or not
*/
bool H3Camera::isPerspectiveProjection () 
{
	return ( this->type == H3COMPONENT_TYPE_CAMERA_PERSPECTIVE );
}

/*
 brief : set forward direction
*/
H3Vector3f H3Camera::getForward () 
{

	H3Quaternion conjugate = this->h3Transform.getRotate().conjugate();

	H3Quaternion Q = this->h3Transform.getRotate().mul( H3Vector3f ( 0.0f , 0.0f , 1.0f ) ).mul( conjugate ) ;

	return H3Vector3f ( Q.x , Q.y , Q.z ) ;


}

/*
 brief : set backward direction
*/
H3Vector3f H3Camera::getBackward () 
{

	H3Quaternion conjugate = this->h3Transform.getRotate().conjugate();

	H3Quaternion Q = this->h3Transform.getRotate().mul( H3Vector3f ( 0.0f , 0.0f , -1.0f ) ).mul( conjugate ) ;

	return H3Vector3f ( Q.x , Q.y , Q.z ) ;

}

/*
 brief : get upward direction
*/
H3Vector3f H3Camera::getUpward () 
{

	H3Quaternion conjugate = this->h3Transform.getRotate().conjugate();

	H3Quaternion Q = this->h3Transform.getRotate().mul( H3Vector3f ( 0.0f , 1.0f , 0.0f ) ).mul( conjugate ) ;

	return H3Vector3f ( Q.x , Q.y , Q.z ) ;


}

/*
 brief : get downward direction
*/
H3Vector3f H3Camera::getDownward () 
{

	H3Quaternion conjugate = this->h3Transform.getRotate().conjugate();

	H3Quaternion Q = this->h3Transform.getRotate().mul( H3Vector3f ( 0.0f , -1.0f , 0.0f ) ).mul( conjugate ) ;

	return H3Vector3f ( Q.x , Q.y , Q.z ) ;

}

/*
 brief : set left direction
*/
H3Vector3f H3Camera::getLeft () 
{

	H3Quaternion conjugate = this->h3Transform.getRotate().conjugate();

	H3Quaternion Q = this->h3Transform.getRotate().mul( H3Vector3f ( -1.0f , 0.0f , 0.0f ) ).mul( conjugate ) ;

	return H3Vector3f ( Q.x , Q.y , Q.z ) ;

}

/*
 brief : set right direction
*/
H3Vector3f H3Camera::getRight () 
{

	H3Quaternion conjugate = this->h3Transform.getRotate().conjugate();

	H3Quaternion Q = this->h3Transform.getRotate().mul( H3Vector3f ( 1.0f , 0.0f , 0.0f ) ).mul( conjugate ) ;

	return H3Vector3f ( Q.x , Q.y , Q.z ) ;

}

/*
 brief :
*/
void H3Camera::rotate ( H3Vector3f aixs , iF32 amout ) 
{
	this->h3Transform.rotate ( H3Quaternion ( aixs , amout ) ) ;
}

/*
 brief :
*/
void H3Camera::lookAt ( H3Vector3f axis ) 
{

	H3Vector3f foward = getForward () ;

}

/*
 brief :
*/
void H3Camera::reset () 
{
	this->projection.identify () ;
	this->eular.identify () ;
	this->h3Transform.reset () ;
}