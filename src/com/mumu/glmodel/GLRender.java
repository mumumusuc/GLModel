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
import android.util.Log;
import android.widget.Toast;

public class GLRender implements GLSurfaceView.Renderer {

	private static final String TAG = "GLRender";

	private final String VERTEX_SHADER_FILE = "shader/vert.glsl";
	private final String FRAGMENT_SHADER_FILE = "shader/frag.glsl";
	private final String MODEL_FILE = "model/cube.obj";
	private final String MTL_FILE = null;

	private int mProgHandler;
	ModelStruct[] models;
	private Context mContext;
	private String mVertexShader, mFragmentShader, mModel;

	public GLRender(Context context) {
		mContext = context;
		AssetManager mAssetMgr = mContext.getAssets();
		if (null == mAssetMgr) {
			Log.e(TAG, "getAssets() return null !");
		}
		mVertexShader = readAssert(mAssetMgr, VERTEX_SHADER_FILE);
		mFragmentShader = readAssert(mAssetMgr, FRAGMENT_SHADER_FILE);
		mModel = readAssert(mAssetMgr, MODEL_FILE);
		Log.i(TAG, "mVertexShader -> " + mVertexShader);
		Log.i(TAG, "mFragmentShader -> " + mFragmentShader);
	}

	private String readAssert(AssetManager as, String file) {
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
		mProgHandler = createProgram(mVertexShader, mFragmentShader);
		Log.i(TAG, "glprogram = " + mProgHandler);
		if (mProgHandler < 0) {
			Log.e(TAG, " error on create glprogram > " + mProgHandler);
			return;
		}
		loadModelMaterial(mModel, null);
		/*
		 * Bitmap[] textures = new Bitmap[1]; textures[0] =
		 * resizeBmp(BitmapFactory.decodeResource(mContext.getResources(),
		 * R.mipmap.tex)); loadBitmapTextrue(textures); for (int i = 0; i <
		 * textures.length; i++) { if (textures[i] != null)
		 * textures[i].recycle(); }
		 */
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
		if (models != null)
			render(mProgHandler, models[0]);
	}

	private void render(int prog, ModelStruct model) {
		render(prog, model._H_VAO, model.mVertexSize, model._H_texture, model.mTextureUnit);
	}

	public void loadModelMaterial(String model, String mtl) {
		new LoadAsyncTask().execute(model, mtl);
	}

	public final native int createProgram(String vert, String frag);

	public final native void resizeWindow(int width, int height);

	private final native ModelStruct[] loadModel(String model, String mtl);

	/* [vao, _size, _texture, _texture_unit, vbo(v),vbo(t),vbo(n)] */
	private final native int[] genRenderArgs(ModelStruct model);

	private final native void render(int prog, int _vao, int _size, int _texture, int _texture_unit);

	private final native int loadBitmapTextrue(Bitmap[] textures);

	public final native void rotateModel(float dx, float dy, float dz, float x, float y, float z);

	public final native void clean();

	static {
		System.loadLibrary("GLRender");
	}

	class LoadAsyncTask extends AsyncTask<String, Integer, String> {
		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			// Toast.makeText(mContext, "load model", Toast.LENGTH_LONG).show();
			Log.i(TAG, "load model");
		}

		@Override
		protected String doInBackground(String... arg0) {
			long s = System.currentTimeMillis();
			models = loadModel(arg0[0], arg0.length > 1 ? arg0[1] : null);
			Log.d(TAG, "models_size=" + models.length);
			if (models.length > 0) {
				Log.d(TAG, models[0].toString());
			}

			return "load done, use " + (System.currentTimeMillis() - s) + " ms";
		}

		@Override
		protected void onPostExecute(String result) {
			super.onPostExecute(result);
			// Toast.makeText(mContext, result, Toast.LENGTH_LONG).show();
			Log.i(TAG, result);
		}
	}

}
