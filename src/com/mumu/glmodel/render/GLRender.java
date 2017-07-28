package com.mumu.glmodel.render;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import com.mumu.glmodel.model.ModelLoader;
import com.mumu.glmodel.model.ModelStruct;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.opengl.GLSurfaceView;
import android.util.Log;

public class GLRender implements GLSurfaceView.Renderer {

	private static final String TAG = "GLRender";

	private final String VERTEX_SHADER_FILE = "shader/vert.glsl";
	private final String FRAGMENT_SHADER_FILE = "shader/frag.glsl";
	private final String MODEL_FILE = "model/toho.obj";
	private final String MTL_FILE = "model/toho.mtl";

	private int mProgHandler = -1;
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
	//	Log.i(TAG, "mVertexShader -> " + mVertexShader);
	//	Log.i(TAG, "mFragmentShader -> " + mFragmentShader);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.i(TAG, "onSurfaceCreated");
		mProgHandler = createProgram(mVertexShader, mFragmentShader);
		if (mProgHandler <= 0) {
			Log.e(TAG, "onSurfaceCreated -> create program failed");
			return;
		}
	}

	public void setRenderModel(ModelStruct[] models) {
		if (models != null) {
			this.models = models;
		/*	for (int i = 0; i < models.length; i++) {
				genRenderParams(models[i]);
				// map_Kd MMD_REIMU\\rm_body.bmp
				String[] names = models[i].mUseMtl.split("\\\\\\\\");
				Log.d(TAG, "usemtl = " + models[i].mUseMtl);
				Log.d(TAG, names[1]);
				String[] name = names[1].split("\\.");
				Log.d(TAG, "name[0] = " + name[0] + ", [1] = " + name[1]);
				Bitmap textures = resizeBmp(
						BitmapFactory.decodeResource(mContext.getResources(), getMipmapId(name[0])));
				models[i].mTextureUnit = i;
				models[i]._H_texture = loadBitmapTextrue(textures, models[i].mTextureUnit);
				textures.recycle();
				Log.i(TAG, models[i].toString());
			}*/
		}
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		resizeWindow(width, height);
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
		render(prog, clear, model._H_vao, model.mCoordinate.mVertexSize / 3, model._H_texture, model._texture_unit,
				model.mMtl.Ns, model.mMtl.Ni, model.mMtl.Ka[0], model.mMtl.Ka[1], model.mMtl.Ka[2], model.mMtl.Kd[0],
				model.mMtl.Kd[1], model.mMtl.Kd[2], model.mMtl.Ks[0], model.mMtl.Ks[1], model.mMtl.Ks[2]);
	}

	public void loadModelMaterial(String model, String mtl) {

	}

	public final native int createProgram(String vert, String frag);

	public final native void resizeWindow(int width, int height);

	private final native void genRenderParams(ModelStruct model);

	private final native void render(int prog, boolean clear, int _vao, int _size, int _texture, int _texture_unit,
			float _Ns, float _Ni, float _Ka0, float _Ka1, float _Ka2, float _Kd0, float _Kd1, float _Kd2, float _Ks0,
			float _Ks1, float _Ks2);

	private final native int loadBitmapTextrue(Bitmap src, int unit);

	public final native void rotateModel(float dx, float dy, float dz, float x, float y, float z);

	public final native void clean();

	static {
		System.loadLibrary("Renderer");
	}
}
