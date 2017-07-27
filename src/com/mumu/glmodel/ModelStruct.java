package com.mumu.glmodel;

public class ModelStruct {
	public int _H_VAO = 0;
	// 顶点
	public int _H_vertex_co = 0;
	public int mVertexSize = 0;
	public float[] mVertexCoBuffer;
	// 纹理
	public int _H_texture = 0;
	public int _H_texture_co = 0;
	public int mTextureUnit = 0;
	public int mTextureSize = 0;
	public float[] mTextureCoBuffer;
	// 法线坐标
	public int _H_normal_co = 0;
	public int mNormalSize = 0;
	public float[] mNormalCoBuffer;
	// 材质
	public String mUseMtl;
	public float Ns = 0;
	public float[] Ka = { 0, 0, 0 };
	public float[] Kd = { 0, 0, 0 };
	public float[] Ks = { 0, 0, 0 };
	public float[] Ke = { 0, 0, 0 };
	public float Ni = 0;
	
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
				.append("mUseMtl = ").append(mUseMtl).append(";\n")
				.append("Ns=").append(Ns).append(";\n")
				.append("Ka=[").append(Ka[0]+","+Ka[1]+","+Ka[2]).append("];\n")
				.append("Kd=[").append(Kd[0]+","+Kd[1]+","+Kd[2]).append("];\n")
				.append("Ks=[").append(Ks[0]+","+Ks[1]+","+Ks[2]).append("];\n")
				.append("Ke=[").append(Ke[0]+","+Ke[1]+","+Ke[2]).append("];\n")
				.append("Ni=").append(Ni).append(";\n")
				.toString();
	}
}
