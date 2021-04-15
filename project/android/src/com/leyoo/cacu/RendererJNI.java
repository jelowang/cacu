package com.leyoo.cacu;

import javax.microedition.khronos.egl.EGLConfig;  
import javax.microedition.khronos.opengles.GL10;   

import android.content.Context;  
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;   
import android.opengl.GLSurfaceView;    
import android.util.Log;  
import android.view.KeyEvent;
import android.view.MotionEvent;

public class RendererJNI implements GLSurfaceView.Renderer {  
     
	public static String sFileDirectory ;    
	public static boolean nativeHasInited = false ;
	
	static { 
        System.loadLibrary("cacu");     
    }            
                   
    private AssetManager mAssetMgr = null;   
    private final String mLogTag = "ndk-build";           
          
    //	native call   
    private static native void nativeGlesInit();    
    private static native void nativeGlesRender();   
    private static native void nativeGlesResize(int width, int height);  
    private static native void nativeSetAssetManager(AssetManager assetMgr);  
    
    private static native void nativeSetApkPath(final String pApkPath);
    
    private static native void nativeTouchesBegin(final int pID[], final float pX[], final float pY[],int totallPoint);
    private static native void nativeTouchesEnd(final int pID[], final float pX[], final float pY[],int totallPoint);
	private static native void nativeTouchesMove(final int pID[], final float pX[], final float pY[],int totallPoint);
	private static native void nativeTouchesCancel(final int pID[], final float pX[], final float pY[],int totallPoint);
	private static native void nativeSensorChanged(final float x , final float y , final float z );
	//	special method for soda
	private static native void nativeLoadObject( String model , String mtl );
	//
	 
	private static native boolean nativeKeyDown(final int pKeyCode);
	private static native void nativeOnPause(); 
	private static native void nativeOnResume();    
          
    public RendererJNI(Context context) {  
    	
    	RendererJNI.sFileDirectory = context.getFilesDir().getAbsolutePath();
		final ApplicationInfo applicationInfo = context.getApplicationInfo();
		RendererJNI.nativeSetApkPath(applicationInfo.sourceDir);
		
        mAssetMgr = context.getAssets();  
        if (null == mAssetMgr) {   
            Log.e(mLogTag, "getAssets() return null !");   
        }  
    }  
          
    @Override  
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {  
        // TODO Auto-generated method stub   
    	RendererJNI.nativeSetAssetManager(mAssetMgr); 
    	RendererJNI.nativeGlesInit();
    	RendererJNI.nativeHasInited = true ;
    }    
    
    @Override   
    public void onSurfaceChanged(GL10 gl, int width, int height) {  
        // TODO Auto-generated method stub  
    	RendererJNI.nativeGlesResize(width, height);  
    }    
      
    @Override  
    public void onDrawFrame(GL10 gl) {  
        // TODO Auto-generated method stub  
    	RendererJNI.nativeGlesRender();          
    }   
     
    //	call from so 
	public static String getCacuWritablePath() {
		return RendererJNI.sFileDirectory;
	}     
	 
	public void handleActionDown(final int pID[], final float pX[], final float pY[],int totallPoint) {
		if (false == RendererJNI.nativeHasInited )
			return ;		
		RendererJNI.nativeTouchesBegin(pID, pX, pY,totallPoint);
		/*
		{
			

			
	        String model = "" 
	            	+"mtllib plane3.mtl\r\n"
	    			+"o Plane\r\n"
	    			+"v -0.184255 1.502510 4.164473\r\n"
	    			+"v 5.786947 1.502510 4.164473\r\n"
	    			+"v -0.184255 1.502510 -1.806730\r\n"
	    			+"v 5.786947 1.502510 -1.806730\r\n"
	    			+"vt 0.000000 1.000000\r\n"
	    			+"vt 0.000000 0.000000\r\n"
	    			+"vt 1.000000 0.000000\r\n"
	    			+"vt 1.000000 1.000000\r\n"
	    			+"vn 0.000000 1.000000 0.000000\r\n"
	    			+"usemtl None\r\n"
	    			+"s off\r\n"
	    			+"f 1/1/1 2/2/1 4/3/1 3/4/1\r\n" ;
	            
	        String mtl = "" 
	    			+"newmtl wire_214228153\r\n"
	    				+"Ns 32\r\n"
	    				+"d 1\r\n"
	    				+"Tr 0\r\n"
	    				+"Tf 1 1 1\r\n"
	    				+"illum 2\r\n"
	    				+"Ka 0.8392 0.8941 0.6000\r\n"
	    				+"Kd 0.8392 0.8941 0.6000\r\n"
	    				+"Ks 0.3500 0.3500 0.3500\r\n"
	    			+"newmtl wire_224086086\r\n"
	    				+"Ns 32\r\n"
	    				+"d 1\r\n"
	    				+"Tr 0\r\n"
	    				+"Tf 1 1 1\r\n"
	    				+"illum 2\r\n"
	    				+"Ka 0.8784 0.3373 0.3373\r\n"
	    				+"Kd 0.8784 0.3373 0.3373\r\n"
	    				+"Ks 0.3500 0.3500 0.3500\r\n"
	    			+"newmtl wire_196088225\r\n"
	    				+"Ns 32\r\n"
	    				+"d 1\r\n"
	    				+"Tr 0\r\n"
	    				+"Tf 1 1 1\r\n"
	    				+"illum 2\r\n"   
	    				+"Ka 0.7686 0.3451 0.8824\r\n"
	    				+"Kd 0.7686 0.3451 0.8824\r\n"
	    				+"Ks 0.3500 0.3500 0.3500\r\n" ; 
	        
	        RendererJNI.loadObject( model , mtl) ;   		
		}	
		*/
	
	
	}  

	public void handleActionUp(final int pID[], final float pX[], final float pY[],int totallPoint) {
		if (false == RendererJNI.nativeHasInited )
			return ;		
		RendererJNI.nativeTouchesEnd(pID, pX, pY,totallPoint);
	}
 
	public void handleActionCancel(final int pID[], final float pX[], final float pY[],int totallPoint) {
		if (false == RendererJNI.nativeHasInited )
			return ;		
		RendererJNI.nativeTouchesCancel(pID, pX, pY,totallPoint);
	}

	public void handleActionMove(final int pID[], final float pX[], final float pY[],int totallPoint) {
		if (false == RendererJNI.nativeHasInited )
			return ;		
		RendererJNI.nativeTouchesMove(pID, pX , pY,totallPoint); 
	}     

	public void handleKeyDown(final int pKeyCode) {
		if (false == RendererJNI.nativeHasInited )
			return ;		
		RendererJNI.nativeKeyDown(pKeyCode);  
	}
	
	public void handleSensorChanged ( final float x, final float y, final float z, final long timestamp ) {
		if (false == RendererJNI.nativeHasInited )
			return ;		
		RendererJNI.nativeSensorChanged ( x, y , z ) ;
	}
   
	public void handleOnPause() { 
		//RendererJNI.nativeOnPause();
	}

	public void handleOnResume() {
		//RendererJNI.nativeOnResume(); 
	}
	
	//	special method for soda
	static void loadObject ( String model , String mtl ) {
		if (false == RendererJNI.nativeHasInited )  
			return ;		
		//	参数 model：模型数据
		//	参数mtl：材质数据
		RendererJNI.nativeLoadObject ( model , mtl ) ;
	}
	//
	  	
	
}  