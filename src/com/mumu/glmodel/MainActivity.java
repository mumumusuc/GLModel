package com.mumu.glmodel;


import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;

public class MainActivity extends Activity implements OnTouchListener{
	
	private static final String TAG = "GL_Activity";
	private static final int CONTEXT_CLIENT_VERSION = 3;  
    private GLSurfaceView mGLSurfaceView;  
    private GLRender mRender ;
    private void initGL(){
    	mGLSurfaceView = new GLSurfaceView(this);
    	if (detectOpenGLES30()) {  
    		  mGLSurfaceView.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION);  
              mGLSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
              mRender = new GLRender(this);
              mGLSurfaceView.setRenderer(mRender); 
              mGLSurfaceView.getHolder().setFormat(PixelFormat.TRANSPARENT);
              mGLSurfaceView.setZOrderOnTop(true);
              mGLSurfaceView.setOnTouchListener(this);
         } else {  
             Log.e(TAG, "OpenGL ES 3.0 not supported on device.  Exiting...");  
             finish();  
         }  
    }
   
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		initGL();
		setContentView(mGLSurfaceView);
	}
    @Override  
    protected void onResume() {  
        super.onResume();  
        mGLSurfaceView.onResume();  
    }  
    @Override
    public void onPause(){
        super.onPause();
        mGLSurfaceView.onPause(); 
    }
    @Override
    public void onDestroy() {
    	mGLSurfaceView.destroyDrawingCache();
    	mGLSurfaceView.queueEvent(new Runnable(){
			@Override
			public void run() {
				mRender.glesClean();
			}});
        super.onDestroy();
    }
    private int deg = 0;	
    private float x = 0;
	@Override
	public boolean onTouch(View arg0, MotionEvent event) {	
		switch(event.getAction()){
		case MotionEvent.ACTION_DOWN:
			x = event.getX();
			break;
		case MotionEvent.ACTION_MOVE:
			if(event.getX() - x > 0)
				deg =(deg+3)%360;
			else
				deg =(deg-3)%360;
			break;
		default :
			return false;
		}
		mGLSurfaceView.queueEvent(new Runnable(){
			@Override
			public void run() {
				
				mRender.glesRotateModel(deg, 0, 1, 0);
			}});  
		return true;
	}
	/********************/
	 private boolean detectOpenGLES30() {  
	        ActivityManager am = (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);  
	        ConfigurationInfo info = am.getDeviceConfigurationInfo();   
	        Log.i(TAG, "support GL_VERSION = "+info.reqGlEsVersion);
	        return (info.reqGlEsVersion >= 0x30000);  
	    }  
}
