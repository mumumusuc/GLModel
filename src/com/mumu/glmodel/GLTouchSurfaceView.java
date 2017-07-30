package com.mumu.glmodel;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class GLTouchSurfaceView extends GLSurfaceView {

	private static final String TAG = GLTouchSurfaceView.class.getName();
	private Renderer mRenderer = null;
	private float deg_x = 0, deg_y = 0, d_z = 0;
	private float x = 0, y = 0, x_1 = 0, y_1 = 0;
	private double d = 0;

	public GLTouchSurfaceView(Context context) {
		super(context);
		init();
	}

	public GLTouchSurfaceView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}

	private void init() {

	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		super.surfaceCreated(holder);
		Log.i(TAG, "surfaceCreated");
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		super.surfaceDestroyed(holder);
		Log.i(TAG, "surfaceDestroyed");
		if (mRenderer instanceof GLRenderer) {
			((GLRenderer) mRenderer).onSurfaceDestroyed();
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		super.surfaceChanged(holder, format, w, h);
		Log.i(TAG, "surfaceChanged");
	}

	@Override
	public void onPause() {
		super.onPause();
		Log.i(TAG, "onPause");
	}

	@Override
	public void onResume() {
		super.onResume();
		Log.i(TAG, "onResume");
	}

	@Override
	public void setRenderer(Renderer renderer) {
		super.setRenderer(renderer);
		mRenderer = renderer;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
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
				d_z = 0;
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
			return true;
		}
		queueEvent(new Runnable() {
			@Override
			public void run() {
				com.mumu.glmodel.render.NativeRenderer.rotateModel(deg_y, deg_x, d_z, 1, 1, 0);
				requestRender();
			}
		});
		return true;
	}

	public interface GLRenderer extends Renderer {
		void onSurfaceDestroyed();
	}
}