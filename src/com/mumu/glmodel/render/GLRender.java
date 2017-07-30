package com.mumu.glmodel.render;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.mumu.glmodel.GLTouchSurfaceView.GLRenderer;
import com.mumu.glmodel.model.ModelLoader;
import com.mumu.glmodel.model.ModelMtl;
import com.mumu.glmodel.model.ModelStruct;
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.Log;

public class GLRender implements GLRenderer {

	private static final String TAG = "GLRender";

	private final String VERTEX_SHADER_FILE = "shader/vert.glsl";
	private final String FRAGMENT_SHADER_FILE = "shader/frag.glsl";

	private int mProgHandler;
	ModelStruct[] models;
	private Context mContext;
	private String mVertexShader, mFragmentShader;

	public GLRender(Context context) {
		mContext = context;
		AssetManager mAssetMgr = mContext.getAssets();
		if (null == mAssetMgr) {
			Log.e(TAG, "getAssets() return null !");
		}
		mVertexShader = ModelLoader.readAssert(mAssetMgr, VERTEX_SHADER_FILE);
		mFragmentShader = ModelLoader.readAssert(mAssetMgr, FRAGMENT_SHADER_FILE);
		// Log.i(TAG, "mVertexShader -> " + mVertexShader);
		// Log.i(TAG, "mFragmentShader -> " + mFragmentShader);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.i(TAG, "onSurfaceCreated");
		mProgHandler = NativeRenderer.createProgram(mVertexShader, mFragmentShader);
		if (mProgHandler <= 0) {
			Log.e(TAG, "onSurfaceCreated -> create program failed");
			return;
		}
	}

	public void setRenderModel(ModelStruct[] models) {
		if (models != null) {
			this.models = models;
			for (int i = 0; i < models.length; i++) {
				NativeRenderer.bindBuffers(models[i].mCoordinate);
				if (models[i].mMtl != null) {
					models[i].mMtl._H_TEXTURE_UNIT = i;
					NativeRenderer.bindTexture(models[i].mMtl);
				}
				if (models[i].mMtl == null) {
					models[i].mMtl = new ModelMtl();
				}
			}
		}
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		NativeRenderer.resizeWindow(width, height);
	}

	@Override
	public void onSurfaceDestroyed() {
		if (models != null) {
			Log.d(TAG, "onSurfaceDestroyed");
			for (int i = 0; i < models.length; i++) {
				NativeRenderer.clear(NativeRenderer.TYPE_BUFFERS, models[i].mCoordinate._H_BUFFERS_ADDR);
				if (models[i].mMtl != null)
					NativeRenderer.clear(NativeRenderer.TYPE_TEXTURE, models[i].mMtl._H_TEXTURE_ADDR);
			}
		}
		models = null;
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		long s = System.currentTimeMillis();
		if (models != null) {
			for (int i = 0; i < models.length; i++) {
				render(mProgHandler, i == 0, models[i]);
			}
		}
		long d = System.currentTimeMillis() - s;
		Log.i(TAG, "fps = " + (1000 / (d == 0 ? 0.001 : d)));
	}

	private void render(int prog, boolean clear, ModelStruct model) {
		NativeRenderer.render(prog, clear, model.mCoordinate._H_VAO, model.mCoordinate.mVertexSize / 3,
				model.mMtl._H_TEXTURE, model.mMtl._H_TEXTURE_UNIT, model.mMtl.Ns, model.mMtl.Ni, model.mMtl.Ka[0],
				model.mMtl.Ka[1], model.mMtl.Ka[2], model.mMtl.Kd[0], model.mMtl.Kd[1], model.mMtl.Kd[2],
				model.mMtl.Ks[0], model.mMtl.Ks[1], model.mMtl.Ks[2]);
	}

}
