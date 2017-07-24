package com.mumu.glmodel;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class MainActivity extends Activity implements OnTouchListener {

	private static final String TAG = "GL_Activity";
	private static final int CONTEXT_CLIENT_VERSION = 3;
	private GLSurfaceView mGLSurfaceView;
	private GLRender mRender;

	private void initGL() {
		mGLSurfaceView = new GLSurfaceView(this);
		if (detectOpenGLES30()) {
			mGLSurfaceView.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION);
			mGLSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
			mRender = new GLRender(this);
			mGLSurfaceView.setRenderer(mRender);
			mGLSurfaceView.getHolder().setFormat(PixelFormat.TRANSPARENT);
			mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
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
	public void onPause() {
		super.onPause();
		mGLSurfaceView.onPause();
	}

	@Override
	public void onDestroy() {
		mGLSurfaceView.destroyDrawingCache();
		mGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				mRender.clean();
			}
		});
		super.onDestroy();
	}

	private float deg_x = 0, deg_y = 0, d_z = 0;
	private float x = 0, y = 0, x_1 = 0, y_1 = 0;
	private double d = 0;

	@Override
	public boolean onTouch(View arg0, MotionEvent event) {
		switch (event.getAction() & MotionEvent.ACTION_MASK) {
		case MotionEvent.ACTION_DOWN:
			deg_x = 0;
			deg_y = 0;
			d_z = 0;
			d = 0;
			x = event.getX();
			y = event.getY();
			break;
		case MotionEvent.ACTION_POINTER_DOWN:
			x_1 = event.getX(1);
			y_1 = event.getY(1);
			break;
		case MotionEvent.ACTION_MOVE:
			int cnt = event.getPointerCount();
			if (cnt == 1) {
				if (event.getX() - x > 30)
					deg_x = 1f;
				else if (event.getX() - x < -30)
					deg_x = -1f;
				if (event.getY() - y > 30)
					deg_y = 1f;
				else if (event.getY() - y < -30)
					deg_y = -1f;
			} else {
				double t = Math.sqrt((x_1 - x) * (x_1 - x) + (y_1 - y) * (y_1 - y));
				d_z = 0 ;
				if (t - d > 20) {
					d_z = 1;
				} else if (t - d < -20) {
					d_z = -1;
				}
				d = t;
				x_1 = event.getX(1);
				y_1 = event.getY(1);
			}
			x = event.getX();
			y = event.getY();
			break;
		case MotionEvent.ACTION_POINTER_UP:
			if (event.getPointerCount() <= 1) {
				d_z = 0;
			}
			break;
		case MotionEvent.ACTION_UP:
				d_z = 0;
			break;
		default:
			return false;
		}

		mGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				mRender.rotateModel(deg_y, deg_x, d_z, 1, 1, 0);
				mGLSurfaceView.requestRender();
			}
		});

		return true;
	}

	/********************/
	private boolean detectOpenGLES30() {
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();
		Log.i(TAG, "support GL_VERSION = " + info.reqGlEsVersion);
		return (info.reqGlEsVersion >= 0x30000);
	}
}
