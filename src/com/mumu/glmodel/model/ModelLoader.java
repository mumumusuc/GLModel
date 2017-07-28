package com.mumu.glmodel.model;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import com.mumu.glmodel.R;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Matrix;

public class ModelLoader {

	public static native ModelCoordinate parseModelCoordinate(String src);

	public static native ModelMtl parseModelMtl(String src);

	public static String readAssert(AssetManager as, String file) {
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
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
				}
			}
		}
		return null;
	}

	public static Bitmap resizeBmp(Bitmap bitmap) {
		if (bitmap.getWidth() > 512 || bitmap.getHeight() > 512) {
			Matrix matrix = new Matrix();
			float scale = 256.0f / (bitmap.getWidth() > bitmap.getHeight() ? bitmap.getWidth() : bitmap.getHeight());
			matrix.postScale(scale, scale);
			Bitmap resizeBmp = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
			return resizeBmp;
		}
		return bitmap;
	}

	public static int getMipmapId(String name) {
		// Log.i(TAG, "for name = " + name);
		int id = -1;
		try {
			Class<?> clz = Class.forName("com.mumu.glmodel.R$mipmap");
			Field _name = clz.getDeclaredField(name);
			id = _name.getInt(R.class);
			// Log.d(TAG, name + "_id = " + id);
		} catch (Exception e) {
			// Log.e(TAG, "getMipmapId:" + e.toString());
		}
		return id;
	}
	
	static {
		System.loadLibrary("ModelLoader");
	}
}
