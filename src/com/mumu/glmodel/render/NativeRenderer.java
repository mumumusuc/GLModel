package com.mumu.glmodel.render;

import com.mumu.glmodel.model.ModelCoordinate;
import com.mumu.glmodel.model.ModelMtl;
import com.mumu.glmodel.model.ModelStruct;

import android.graphics.Bitmap;

public class NativeRenderer {

	public static final int TYPE_BUFFERS = 1000;
	public static final int TYPE_TEXTURE = 1001;

	public static final native int createProgram(String vert, String frag);

	public static final native void bindBuffers(ModelCoordinate c);

	public static final native void bindTexture(ModelMtl m);

	public static final native void render(int prog, boolean clear, int _vao, int _size, int _texture,
			int _texture_unit, float _Ns, float _Ni, float _Ka0, float _Ka1, float _Ka2, float _Kd0, float _Kd1,
			float _Kd2, float _Ks0, float _Ks1, float _Ks2);

	public static final native int loadBitmapTextrue(Bitmap src, int unit);

	public static final native void resizeWindow(int width, int height);

	public static final native void rotateModel(float dx, float dy, float dz, float x, float y, float z);

	public static final native void clear(int type, int addr);

	static {
		System.loadLibrary("Renderer");
	}
}
