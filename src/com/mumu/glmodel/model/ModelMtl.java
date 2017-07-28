package com.mumu.glmodel.model;

import android.graphics.Bitmap;

public class ModelMtl {
	public String mUseMtl;
	public Bitmap mTexture;
	public float Ns = 0;
	public float[] Ka = { 0, 0, 0 };
	public float[] Kd = { 0, 0, 0 };
	public float[] Ks = { 0, 0, 0 };
	public float Ni = 0;
	
	@Override
	public String toString(){
		return new StringBuilder()
				.append("Ns=").append(Ns).append(";\n")
				.append("Ka=[").append(Ka[0]+","+Ka[1]+","+Ka[2]).append("];\n")
				.append("Kd=[").append(Kd[0]+","+Kd[1]+","+Kd[2]).append("];\n")
				.append("Ks=[").append(Ks[0]+","+Ks[1]+","+Ks[2]).append("];\n")
				.append("Ni=").append(Ni).append(";\n")
				.append("mUseMtl = ").append(mUseMtl).append(";\n")
				.append("mTexture = ").append(mTexture).append(";\n")
				.toString();
	}
}
