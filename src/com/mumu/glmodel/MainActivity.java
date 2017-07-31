package com.mumu.glmodel;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.mumu.glmodel.model.ModelCoordinate;
import com.mumu.glmodel.model.ModelLoader;
import com.mumu.glmodel.model.ModelMtl;
import com.mumu.glmodel.model.ModelStruct;
import com.mumu.glmodel.render.GLRender;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnItemClickListener {

	private static final String TAG = "GL_Activity";
	private static final int CONTEXT_CLIENT_VERSION = 3;
	private GLSurfaceView mGLSurfaceView;
	private ListView mListView;
	private GLRender mRender;
	private ListAdapter mAdapter;
	private List<String> mList = new ArrayList<String>() {
		private static final long serialVersionUID = -8386121303628702522L;
		{
			add("ball");
			add("cube");
			add("monky");
			add("splot");
			add("toho");
			add("car");
		}
	};
	private Map<Integer, ModelStruct[]> mModelsMap;

	private void initGL() {
		mGLSurfaceView = (GLSurfaceView) findViewById(R.id.surface_view);
		mGLSurfaceView.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION);
		mGLSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		mRender = new GLRender(this);
		mGLSurfaceView.setRenderer(mRender);
		mGLSurfaceView.getHolder().setFormat(PixelFormat.TRANSPARENT);
		// mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		mGLSurfaceView.setZOrderOnTop(true);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (detectOpenGLES30()) {
		} else {
			Log.e(TAG, "OpenGL ES 3.0 not supported on device.  Exiting...");
			finish();
		}
		setContentView(R.layout.activity_main);
		initGL();
		mListView = (ListView) findViewById(R.id.model_list);
		mAdapter = new ListAdapter();
		mAdapter.setList(mList);
		mListView.setAdapter(mAdapter);
		mListView.setOnItemClickListener(this);
		mModelsMap = new HashMap<Integer, ModelStruct[]>();
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
		super.onDestroy();
		// mGLSurfaceView.destroyDrawingCache();
	}

	/********************/
	private boolean detectOpenGLES30() {
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();
		Log.i(TAG, "support GL_VERSION = " + info.reqGlEsVersion);
		return (info.reqGlEsVersion >= 0x30000);
	}

	@Override
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		mGLSurfaceView.setVisibility(View.VISIBLE);
		mListView.setVisibility(View.INVISIBLE);
		new LoadAsyncTask().execute(arg2);
	}

	class LoadAsyncTask extends AsyncTask<Integer, Integer, ModelStruct[]> {
		long s = 0;

		@Override
		protected ModelStruct[] doInBackground(Integer... arg) {
			s = System.currentTimeMillis();
			ModelStruct[] mModels = null;
			if (mModelsMap.containsKey(arg[0])) {
				mModels = mModelsMap.get(arg[0]);
			} else {
				String co = ModelLoader.readAssert(getAssets(), "model/" + mList.get(arg[0]) + ".obj");
				String mtl = ModelLoader.readAssert(getAssets(), "model/" + mList.get(arg[0]) + ".mtl");
				ModelCoordinate[] cos = null;
				cos = ModelLoader.parseModelCoordinate(co);
				Log.d(TAG, "load coordinate use" + (System.currentTimeMillis() - s) + "ms");
				ModelMtl[] mtls = ModelLoader.parseModelMtl(mtl);
				Log.d(TAG, "load mtl use" + (System.currentTimeMillis() - s) + "ms");
				if (mtls != null) {
					Log.d(TAG, "mtls_size=" + mtls.length);
					for (int i = 0; i < mtls.length; i++) {
						String name = mtls[i].mTextureName.split("\\\\\\\\")[1].split("\\.")[0];
						mtls[i].mTexture = ModelLoader.loadBmp(getResources(), ModelLoader.getMipmapId(name));
						Log.d(TAG, "mtl[" + i + "] = " + mtls[i].toString());
					}
				}
				if (cos != null) {
					Log.d(TAG, "cooridinates_size=" + cos.length);
					for (int i = 0; i < cos.length; i++) {
						Log.d(TAG, "cooridinate[" + i + "] = " + cos[i].toString());
					}
					mModels = new ModelStruct[cos.length];
					for (int i = 0; i < cos.length; i++) {
						mModels[i] = new ModelStruct(cos[i], null);
						if (mtls != null) {
							String useMtl = cos[i].mUseMtl;
							for (int j = 0; j < mtls.length; j++) {
								if (useMtl.equals(mtls[j].mNewMtl)) {
									mModels[i].mMtl = mtls[j];
								}
							}
						}
					}
					mModelsMap.put(arg[0], mModels);
				}
			}
			return mModels;
		}

		@Override
		protected void onPostExecute(final ModelStruct[] result) {
			super.onPostExecute(result);
			Toast.makeText(MainActivity.this, "load done, use " + (System.currentTimeMillis() - s) + " ms",
					Toast.LENGTH_SHORT).show();
			if (result != null)
				mGLSurfaceView.queueEvent(new Runnable() {
					@Override
					public void run() {
						mRender.setRenderModel(result);
						mGLSurfaceView.requestRender();
					}
				});
		}
	}

	@Override
	public void onBackPressed() {
		if (mGLSurfaceView.getVisibility() == View.VISIBLE) {
			mGLSurfaceView.setVisibility(View.INVISIBLE);
			mListView.setVisibility(View.VISIBLE);
		} else {
			super.onBackPressed();
		}
	}
}
