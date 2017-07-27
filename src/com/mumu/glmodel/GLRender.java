package com.mumu.glmodel;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class GLRender implements GLSurfaceView.Renderer {

	private static final String TAG = "GLRender";

	private final String VERTEX_SHADER_FILE = "shader/vert.glsl";
	private final String FRAGMENT_SHADER_FILE = "shader/frag.glsl";
	private final String MODEL_FILE = "model/toho.obj";
	private final String MTL_FILE = "model/toho.mtl";

	private int mProgHandler = -1;
	ModelStruct[] models;
	private Context mContext;
	private String mVertexShader, mFragmentShader, mModel, mMtl;

	public GLRender(Context context) {
		mContext = context;
		AssetManager mAssetMgr = mContext.getAssets();
		if (null == mAssetMgr) {
			Log.e(TAG, "getAssets() return null !");
		}
		mVertexShader = readAssert(mAssetMgr, VERTEX_SHADER_FILE);
		mFragmentShader = readAssert(mAssetMgr, FRAGMENT_SHADER_FILE);
		//mModel = readAssert(mAssetMgr, MODEL_FILE);
		//mMtl = readAssert(mAssetMgr, MTL_FILE);
		Log.i(TAG, "mVertexShader -> " + mVertexShader);
		Log.i(TAG, "mFragmentShader -> " + mFragmentShader);
		// loadModelMaterial(mModel, mMtl);
	}

	public String readAssert(AssetManager as, String file) {
		StringBuilder sb = new StringBuilder();
		InputStream is = null;
		byte[] buffer = new byte[1024];
		try {
			is = as.open(file);
			int size = 0;
			while ((size = is.read(buffer)) > 0) {
				for (int i = 0; i < size; i++)
					sb.append((char) buffer[i]);
			}
			return sb.toString();
		} catch (IOException e) {
			Log.e(TAG, Log.getStackTraceString(e));
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
					Log.e(TAG, Log.getStackTraceString(e));
				}
			}
		}
		return null;
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.i(TAG, "onSurfaceCreated -> glprogram = " + mProgHandler);
		if (mProgHandler <= 0) {
			mProgHandler = createProgram(mVertexShader, mFragmentShader);
		}
		if (mProgHandler <= 0) {
			Log.e(TAG, "onSurfaceCreated -> create program failed");
			return;
		}
		// models = loadModel(mModel, mMtl);

	}

	public void setRenderModel(ModelStruct[] models) {
		if (models != null) {
			this.models = models;
			for (int i = 0; i < models.length; i++) {
				genRenderParams(models[i]);
				// Bitmap textures =
				// resizeBmp(BitmapFactory.decodeResource(mContext.getResources(),
				// R.mipmap.tex));
				// models[0].mTextureUnit = 0;
				// models[0]._H_texture = loadBitmapTextrue(textures,
				// models[0].mTextureUnit);
				// textures.recycle();
				Log.i(TAG, models[i].toString());
			}
		}
	}

	private Bitmap resizeBmp(Bitmap bitmap) {
		if (bitmap.getWidth() > 512 || bitmap.getHeight() > 512) {
			Matrix matrix = new Matrix();
			float scale = 256.0f / (bitmap.getWidth() > bitmap.getHeight() ? bitmap.getWidth() : bitmap.getHeight());
			matrix.postScale(scale, scale);
			Bitmap resizeBmp = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
			return resizeBmp;
		}
		return bitmap;
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		resizeWindow(width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		// long s = System.currentTimeMillis();
		if (models != null) {
			for (int i = 0; i < models.length; i++) {
				render(mProgHandler, models[i]);
			}
		}
		// long d = System.currentTimeMillis() - s;
		// Log.i(TAG, "fps = " + (1000 / (d == 0 ? 0.001 : d)));
	}

	private void render(int prog, ModelStruct model) {
		render(prog, model._H_VAO, model.mVertexSize / 3, model._H_texture, model.mTextureUnit, model.Ns, model.Ni,
				model.Ka[0], model.Ka[1], model.Ka[2], model.Kd[0], model.Kd[1], model.Kd[2], model.Ks[0], model.Ks[1],
				model.Ks[2]);
	}

	public void loadModelMaterial(String model, String mtl) {

	}

	public final native int createProgram(String vert, String frag);

	public final native void resizeWindow(int width, int height);

	public final native ModelStruct[] loadModel(String model, String mtl);

	private final native void genRenderParams(ModelStruct model);

	private final native void render(int prog, int _vao, int _size, int _texture, int _texture_unit, float _Ns,
			float _Ni, float _Ka0, float _Ka1, float _Ka2, float _Kd0, float _Kd1, float _Kd2, float _Ks0, float _Ks1,
			float _Ks2);

	private final native int loadBitmapTextrue(Bitmap src, int unit);

	public final native void rotateModel(float dx, float dy, float dz, float x, float y, float z);

	public final native void clean();

	static {
		System.loadLibrary("GLRender");
	}
}
