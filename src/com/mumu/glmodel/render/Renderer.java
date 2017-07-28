package com.mumu.glmodel.render;

import com.mumu.glmodel.model.ModelStruct;

import android.graphics.Bitmap;

public class Renderer {
	public final native int createProgram(String vert, String frag);

	private final native void genRenderParams(ModelStruct s);

	private final native void render(int prog, boolean clear, int _vao, int _size, int _texture, int _texture_unit,
			float _Ns, float _Ni, float _Ka0, float _Ka1, float _Ka2, float _Kd0, float _Kd1, float _Kd2, float _Ks0,
			float _Ks1, float _Ks2);

	private final native int loadBitmapTextrue(Bitmap src, int unit);

	public final native void resizeWindow(int width, int height);

	public final native void rotateModel(float dx, float dy, float dz, float x, float y, float z);

	public final native void clean();
}
