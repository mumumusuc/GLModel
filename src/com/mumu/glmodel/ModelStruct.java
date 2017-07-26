package com.mumu.glmodel;

public class ModelStruct {
	public int _H_VAO;
	public String mUseMtl;
	// 顶点
	public int _H_vertex_co;
	public int mVertexSize;
	public float[] mVertexCoBuffer;
	// 纹理
	public int _H_texture;
	public int _H_texture_co;
	public int mTextureName;
	public int mTextureUnit;
	public int mTextureSize;
	public float[] mTextureCoBuffer;
	// 法线坐标
	public int _H_normal_co;
	public int mNormalSize;
	public float[] mNormalCoBuffer;
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
				.append("_H_vertex = ").append(_H_vertex_co).append(";\n")
				.append("mVertexSize = ").append(mVertexSize).append(",")
				.append(mVertexCoBuffer==null?0:mVertexCoBuffer.length).append(";\n")
				.append("_H_texture = ").append(_H_texture_co).append(";\n")
				.append("mTextureSize = ").append(mTextureSize).append(",")
				.append(mTextureCoBuffer==null?0:mTextureCoBuffer.length).append(";\n")
				.append("_H_normal = ").append(_H_normal_co).append(";\n")
				.append("mNormalSize = ").append(mNormalSize).append(",")
				.append(mNormalCoBuffer==null?0:mNormalCoBuffer.length).append(";\n")
				.toString();
	}
}
