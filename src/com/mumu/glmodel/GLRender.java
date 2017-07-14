package com.mumu.glmodel;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.opengl.GLSurfaceView;
import android.util.Log;

public class GLRender implements GLSurfaceView.Renderer {

	private static final String TAG = "GLRender";

	private final String VERTEX_SHADER_FILE = "shader/vs.glsl";
	private final String FRAGMENT_SHADER_FILE = "shader/fs.glsl";
	private final String MODEL_FILE = "model/cube";

	private Context mContext;
	private String mVertexShader, mFragmentShader,mModel;

	public GLRender(Context context) {
		mContext = context;
		AssetManager mAssetMgr = mContext.getAssets();
		if (null == mAssetMgr) {
			Log.e(TAG, "getAssets() return null !");
		}
		mVertexShader = readAssert(mAssetMgr, VERTEX_SHADER_FILE);
		mFragmentShader = readAssert(mAssetMgr, FRAGMENT_SHADER_FILE);
		mModel = readAssert(mAssetMgr,MODEL_FILE);
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
		int mProgHandler = createProgram();
		if (mProgHandler < 0) {
			Log.e(TAG, " error on create glprogram");
			return;
		}
		initShader(mProgHandler, mVertexShader, mFragmentShader);
		loadModel(mModel);

/*		Bitmap[] textures = new Bitmap[24];
		textures[0] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.bangs));
		textures[1] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.face));
		textures[2] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.dark));
		textures[3] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.sleeve));
		textures[4] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.skirt));
		textures[5] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.st));
		textures[6] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.stw));
		textures[7] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.glow));
		textures[8] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.pantsu));
		textures[9] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.tie));
		textures[10] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.glowst));
		textures[11] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.back));
		textures[12] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.top));
		textures[13] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.pin));
		textures[14] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.headset));
		textures[15] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.tail));
		textures[16] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.eye));
		textures[17] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.ribbon));
		textures[18] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.b));
		textures[19] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.skin));
		textures[20] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.ribbon1));
		textures[21] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.squares));
		textures[22] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.nail));
		textures[23] = resizeBmp(BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.on));
		loadBitmapTextrue(textures);
		for (int i = 0; i < textures.length; i++) {
			if (textures[i] != null)
				textures[i].recycle();
		}*/
	}

	private Bitmap resizeBmp(Bitmap bitmap) {
		if (bitmap.getWidth() > 512 || bitmap.getHeight() > 512) {
			Matrix matrix = new Matrix();
			float scale = 256.0f / (bitmap.getWidth() > bitmap.getHeight() ? bitmap.getWidth() : bitmap.getHeight());
			matrix.postScale(scale, scale); // 长和宽放大缩小的比例
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
		render();
	}

	public final native int createProgram();

	public final native void resizeWindow(int width, int height);

	public final native void initShader(int prog, String vertex, String fragment);

	public final native void loadModel(String model);

	public final native void render();

	public final native int loadBitmapTextrue(Bitmap[] textures);

	public final native void glesRotateModel(float deg, float x, float y, float z);

	public final native void glesClean();
	
	static {
		System.loadLibrary("GLRender");
	}


}
