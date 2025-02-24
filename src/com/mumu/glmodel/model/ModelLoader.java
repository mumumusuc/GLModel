package com.mumu.glmodel.model;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import com.mumu.glmodel.R;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.BitmapFactory.Options;
import android.graphics.Matrix;

public class ModelLoader {

	public static native ModelCoordinate[] parseModelCoordinate(String src);

	public static native ModelMtl[] parseModelMtl(String src);

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

	public static Bitmap loadBmp(Resources res, int resID) {
		Options opt = new Options();
		opt.inJustDecodeBounds = true;
		BitmapFactory.decodeResource(res, resID, opt);
		if (opt.outWidth > 256 || opt.outHeight > 256) {
			opt.inScaled = true;
			int resize = Math.max(opt.outWidth / 256, opt.outHeight / 256);
			opt.inSampleSize = resize / 2 * 2 + 1;
			opt.inJustDecodeBounds = false;
		}
		return BitmapFactory.decodeResource(res, resID, opt);
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
