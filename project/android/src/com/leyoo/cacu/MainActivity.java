package com.leyoo.cacu;

//opengl es
import android.app.Activity;  
import android.app.ActivityManager;  
import android.content.Context;  
import android.content.pm.ConfigurationInfo;  
import android.opengl.GLSurfaceView;  
import android.os.Bundle;  
import android.util.AttributeSet;
import android.util.Log;  

import android.widget.TextView;
import android.os.Bundle;
import android.app.Activity;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MotionEvent;
  
public class MainActivity extends Activity {
                   	    
	private H3Accelerometer mAccelerometer ;
    private H3GLSurfaceView mGLSurfaceView;    
  
    	@Override     
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);                                  
		   
		mGLSurfaceView = new H3GLSurfaceView(this);  
        if (detectOpenGLES30()) {  
        	mGLSurfaceView.setEGLContextClientVersion(3); 
        	Log.e("H3", "OpenGL ES 3.0 settled");
        } else {      
    	   mGLSurfaceView.setEGLContextClientVersion(2);  
           Log.e("H3", "OpenGL ES 2.0 settled");    
        } 		  
		mGLSurfaceView.setRender( new RendererJNI(this) ) ;  
        setContentView(mGLSurfaceView);  
        
        mAccelerometer = new H3Accelerometer(this) ;
        mAccelerometer.setGLSurface( mGLSurfaceView ) ; 
        
        mAccelerometer.enable() ;   
        
            	   
	}  
	
	public boolean onCreateOptionsMenu(Menu menu) { 
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.activity_main, menu);    
		return true;
	}   
	
	
	@Override  
	protected void onResume() {  
		// TODO Auto-generated method stub  
		super.onResume();  
		//mGLSurfaceView.onResume();    
	}  
	      
	@Override  
	protected void onPause() {   
		// TODO Auto-generated method stub   
		super.onPause();   
		//mGLSurfaceView.onPause();  
	}    
	     
	public boolean detectOpenGLES30() {  
		ActivityManager am = (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);  
		ConfigurationInfo info = am.getDeviceConfigurationInfo();  
		return (info.reqGlEsVersion >= 0x30000);  
	}     
	
}
