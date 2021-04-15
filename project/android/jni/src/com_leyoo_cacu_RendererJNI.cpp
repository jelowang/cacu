#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>
#include <android/asset_manager_jni.h>  
#include <android/log.h>   
#include "JniHelper.h"
#include <jni.h>
//#include "StudioWindow.h"
#include "SodaDemo.h"
#include "H3Delegate.h"

#define LOG_TAG "ndk-build"  
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)  
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)  
  
static string g_apkPath ;
AAssetManager* g_pAssetManager = NULL;  

#define ENABLE_H3

extern "C" {
  
//*********************************************************************************
//
JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeSetAssetManager(JNIEnv *env, jobject self, jobject assetManager)
{
     if (assetManager && env)  
     {
         //LOGI("before AAssetManager_fromJava");  
         g_pAssetManager = AAssetManager_fromJava(env, assetManager);  
         //LOGI("after AAssetManager_fromJava");
         if (NULL == g_pAssetManager)
         {
             LOGE("AAssetManager_fromJava() return null !");  
         }  
     }
     else  
     {
         LOGE("assetManager is null !"); 
     }  

}

//*********************************************************************************  
//
JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeGlesInit(JNIEnv *pEnv, jobject obj)
{  
#ifdef ENABLE_H3    
	H3Delegate::getInstance()->init ( 60 ) ;   
	SodaDemo* sodaDemo = MMC_NEW SodaDemo() ;
	sodaDemo->loadFile ( "models/first.map" ) ; 
	H3Delegate::getInstance()->start( sodaDemo ) ;
	H3Delegate::getInstance()->resize(640, 960) ;
#endif
}  

//*********************************************************************************  
//  
JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeGlesRender(JNIEnv *pEnv, jobject obj)
{
#ifdef ENABLE_H3

	H3Delegate::getInstance()->update( 0.016f ) ;
#endif
}

//*********************************************************************************  
//
JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeGlesResize(JNIEnv *pEnv, jobject obj, jint width, jint height)
{  
#ifdef ENABLE_H3
	H3Delegate::getInstance()->resize(width, height) ;
#endif	
	LOGE("ViewPort : %d,%d",width,height) ;
}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeSetApkPath(JNIEnv*  env, jobject thiz, jstring apkPath) {
	
	//	something like "/data/app/com.leyoo.cacu-1.apk"
	g_apkPath = JniHelper::jstring2string(apkPath);
	//LOGE("g_apkPath %s",g_apkPath.data()) ;
	 	  
}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeTouchesBegin(JNIEnv *pEnv, jobject obj, jintArray pid , jfloatArray x , jfloatArray y , int totallPoint )
{

	int* parray = pEnv->GetIntArrayElements(pid,NULL);
	float* xarray = pEnv->GetFloatArrayElements(x,NULL);
	float* yarray = pEnv->GetFloatArrayElements(y,NULL);

	//LOGE("Totall Point %d",totallPoint) ;
	//LOGE("pid len = %d",pEnv->GetArrayLength(pid)) ;bb
	//LOGE("x len = %d",pEnv->GetArrayLength(x)) ;
	//LOGE("y len = %d",pEnv->GetArrayLength(y)) ;
	//LOGE( "%d , %f , %f" , parry[0] , xarry[0] , yarry[0] ) ;
	
	H3MouseEvent m ;

	m.e |= H3MouseEvent_Click ;

	for ( int i = 0 ; i < totallPoint && sizeof(m.point.x) / sizeof(int) ; i ++ ) {
		m.point.id[i] = parray[i] ;
		m.point.x[i] = (int )xarray[i] ;
		m.point.y[i] = (int )yarray[i] ;
		m.point.totall ++ ;
	}

	m.e |= H3MouseEvent_Right_Button ;

#ifdef ENABLE_H3	
	H3Delegate::getInstance()->mouseEvent ( m ) ;
#endif

	pEnv->ReleaseIntArrayElements(pid,parray,0);
	pEnv->ReleaseFloatArrayElements(x,xarray,0);
	pEnv->ReleaseFloatArrayElements(y,yarray,0);


}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeTouchesEnd(JNIEnv *pEnv, jobject obj, jintArray pid , jfloatArray x , jfloatArray y , int totallPoint )
{

	int* parray = pEnv->GetIntArrayElements(pid,NULL);
	float* xarray = pEnv->GetFloatArrayElements(x,NULL);
	float* yarray = pEnv->GetFloatArrayElements(y,NULL);

	//LOGE("Totall Point %d",totallPoint) ;
	//LOGE("pid len = %d",pEnv->GetArrayLength(pid)) ;
	//LOGE("x len = %d",pEnv->GetArrayLength(x)) ;
	//LOGE("y len = %d",pEnv->GetArrayLength(y)) ;
	//LOGE( "%d , %f , %f" , parry[0] , xarry[0] , yarry[0] ) ;

	H3MouseEvent m ;

	m.e |= H3MouseEvent_Release ;

	for ( int i = 0 ; i < totallPoint && sizeof(m.point.x) / sizeof(int) ; i ++ ) {
		m.point.id[i] = parray[i] ;
		m.point.x[i] = (int )xarray[i] ;
		m.point.y[i] = (int )yarray[i] ;
		m.point.totall ++ ;
	}

	m.e |= H3MouseEvent_Right_Button ;

#ifdef ENABLE_H3	
	H3Delegate::getInstance()->mouseEvent ( m ) ;
#endif

	pEnv->ReleaseIntArrayElements(pid,parray,0);
	pEnv->ReleaseFloatArrayElements(x,xarray,0);
	pEnv->ReleaseFloatArrayElements(y,yarray,0);

}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeTouchesCancel(JNIEnv *pEnv, jobject obj, jintArray pid , jfloatArray x , jfloatArray y , int totallPoint )
{
	//LOGE( "%d , %f , %f" , pid , x , y ) ;
}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeTouchesMove(JNIEnv *pEnv, jobject obj, jintArray pid , jfloatArray x , jfloatArray y , int totallPoint )
{

	int* parray = pEnv->GetIntArrayElements(pid,NULL);
	float* xarray = pEnv->GetFloatArrayElements(x,NULL);
	float* yarray = pEnv->GetFloatArrayElements(y,NULL);

	//LOGE("Totall Point %d",totallPoint) ;
	//LOGE("pid len = %d",pEnv->GetArrayLength(pid)) ;
	//LOGE("x len = %d",pEnv->GetArrayLength(x)) ;
	//LOGE("y len = %d",pEnv->GetArrayLength(y)) ;
	//LOGE( "%d , %f , %f" , parry[0] , xarry[0] , yarry[0] ) ;

	H3MouseEvent m ;

	m.e |= H3MouseEvent_Moving ;

	for ( int i = 0 ; i < totallPoint && sizeof(m.point.x) / sizeof(int) ; i ++ ) {
		m.point.id[i] = parray[i] ;
		m.point.x[i] = (int )xarray[i] ;
		m.point.y[i] = (int )yarray[i] ;
		m.point.totall ++ ;
	}

	m.e |= H3MouseEvent_Right_Button ;

#ifdef ENABLE_H3		
	H3Delegate::getInstance()->mouseEvent ( m ) ;
#endif

	pEnv->ReleaseIntArrayElements(pid,parray,0);
	pEnv->ReleaseFloatArrayElements(x,xarray,0);
	pEnv->ReleaseFloatArrayElements(y,yarray,0);

}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeSensorChanged(JNIEnv *pEnv, jobject obj, jfloat x , jfloat y , jfloat z )
{
	H3AccelerometerEvent m ;

	m.x = x ;
	m.y = y ;
	m.z = z ;

#ifdef ENABLE_H3		
	H3Delegate::getInstance()->accelerometerEvent ( m ) ;
#endif

}

//	special method for soda
JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeLoadObject(JNIEnv *pEnv, jobject obj, jstring model , jstring mtl )
{

	char* modelString = pEnv->GetStringUTFChars(model, 0);
	char* mtlString = pEnv->GetStringUTFChars(mtl, 0);

#ifdef ENABLE_H3		
	H3Delegate::getInstance()->loadObject ( modelString , mtlString ) ;
#endif

	pEnv->ReleaseStringUTFChars(model,modelString);
	pEnv->ReleaseStringUTFChars(mtl,mtlString);

}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeOnPause()
{
}

JNIEXPORT void JNICALL Java_com_leyoo_cacu_RendererJNI_nativeOnResume()
{
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

}// end extern "C"

std::string getFileDirectory_to_JAVA() 
{
	
	//	something like "/data/data/com.leyoo.cacu/files"
	JniMethodInfo t;
    std::string ret("");

    if (JniHelper::getStaticMethodInfo(t,"com/leyoo/cacu/RendererJNI","getCacuWritablePath", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }

    return ret;
}

std::string JNIAndroidGetApkPath () 
{
	return g_apkPath ;
}
