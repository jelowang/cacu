package com.leyoo.cacu;

import android.app.ActivityManager;
import android.content.Context;  
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;   
import android.opengl.GLSurfaceView;    
import android.util.Log;  
import android.view.KeyEvent;
import android.view.MotionEvent;

public class H3GLSurfaceView extends GLSurfaceView
{
	private RendererJNI mRendererJni ; 
	
	public H3GLSurfaceView(final Context context) {
		super(context);
		this.initView () ;
	} 
	
	public void initView () {

		this.setFocusableInTouchMode(true);	
		//setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY); 
	}
	
	public void setRender ( RendererJNI render ) {
		this.mRendererJni = render ;		
        this.setRenderer( mRendererJni );    
	}
	 
	@Override
	public boolean onTouchEvent(final MotionEvent pMotionEvent) {
		// these data are used in ACTION_MOVE and ACTION_CANCEL
		final int pointerNumber = pMotionEvent.getPointerCount();
		final int[] ids = new int[pointerNumber];
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];

		for (int i = 0; i < pointerNumber; i++) {
			ids[i] = pMotionEvent.getPointerId(i);
			xs[i] = pMotionEvent.getX(i);
			ys[i] = pMotionEvent.getY(i);
		}
		
		switch (pMotionEvent.getAction() & MotionEvent.ACTION_MASK) {
			case MotionEvent.ACTION_POINTER_DOWN:
				final int indexPointerDown = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerDown = pMotionEvent.getPointerId(indexPointerDown);
				final float xPointerDown = pMotionEvent.getX(indexPointerDown);
				final float yPointerDown = pMotionEvent.getY(indexPointerDown);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//mRendererJni.handleActionDown(idPointerDown, xPointerDown, yPointerDown);
						mRendererJni.handleActionDown(ids, xs, ys,pointerNumber);
					}
		 		});
				break;

			case MotionEvent.ACTION_DOWN:
		
				// there are only one finger on the screen
				final int idDown = pMotionEvent.getPointerId(0);
				final float xDown = xs[0];
				final float yDown = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//mRendererJni.handleActionDown(idDown, xDown, yDown);
						mRendererJni.handleActionDown(ids, xs, ys,pointerNumber);
					}
				});
				break;

			case MotionEvent.ACTION_MOVE:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//mRendererJni.handleActionMove(ids, xs, ys);
						mRendererJni.handleActionMove(ids, xs, ys,pointerNumber);
					}
				});
				break;

			case MotionEvent.ACTION_POINTER_UP:
				final int indexPointUp = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerUp = pMotionEvent.getPointerId(indexPointUp);
				final float xPointerUp = pMotionEvent.getX(indexPointUp);
				final float yPointerUp = pMotionEvent.getY(indexPointUp);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//mRendererJni.handleActionUp(idPointerUp, xPointerUp, yPointerUp);
						mRendererJni.handleActionUp(ids, xs, ys,pointerNumber);
					}
				});
				break;

			case MotionEvent.ACTION_UP:
				// there are only one finger on the screen
				final int idUp = pMotionEvent.getPointerId(0);
				final float xUp = xs[0];
				final float yUp = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//mRendererJni.handleActionUp(idUp, xUp, yUp);
						mRendererJni.handleActionUp(ids, xs, ys,pointerNumber);
					}
				});
				break;
 
			case MotionEvent.ACTION_CANCEL:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//mRendererJni.handleActionCancel(ids, xs, ys);
						mRendererJni.handleActionCancel(ids, xs, ys,pointerNumber);
					}
				});  
				break;
		}	
		
		return true ;
		
	}	
	
    public void queueAccelerometer(final float x, final float y, final float z, final long timestamp) {	
    	this.queueEvent(new Runnable() {
		@Override
		    public void run() {
			mRendererJni.handleSensorChanged(x, y, z, timestamp);
		}
	    });
    }
    

}