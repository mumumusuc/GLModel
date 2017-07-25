package com.mumu.glmodel;

public class ModelStruct {
	public int _H_VAO;
	// 顶点
	public int _H_vertex;
	public int mVertexSize;
	// 纹理
	public int _H_texture;
	public int mTextureUnit;
	// 法线坐标
	public int _H_normal;
	// 材质
	public float Ns;
	public float[] Ka;
	public float[] Kd;
	public float[] Ks;
	public float[] Ke;
	public float Ni;
	public float d;
	
	@Override
	public String toString(){
		return new StringBuilder()
				.append("_H_vertex = ").append(_H_vertex).append(";\n")
				.append("_H_texture = ").append(_H_texture).append(";\n")
				.append("_H_normal = ").append(_H_normal).append(";\n")
				.toString();
	}
}
