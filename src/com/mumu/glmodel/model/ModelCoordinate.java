package com.mumu.glmodel.model;

public class ModelCoordinate {
	public int _H_VAO;
	public int _H_VERTEX_CO;
	public int _H_TEXTURE_CO;
	public int _H_NORMAL_CO;
	public int _H_BUFFERS_ADDR;
	
	public String mUseMtl;

	public int mVertexSize;
	public float[] mVertexCoBuffer;

	public int mTextureSize;
	public float[] mTextureCoBuffer;

	public int mNormalSize;
	public float[] mNormalCoBuffer;
	
	@Override public String toString(){
		return new StringBuilder()
				.append("_H_VAO = ").append(_H_VAO).append(";\n")
				.append("_H_VERTEX_CO = ").append(_H_VERTEX_CO).append(";\n")
				.append("_H_TEXTURE_CO = ").append(_H_TEXTURE_CO).append(";\n")
				.append("_H_NORMAL_CO = ").append(_H_NORMAL_CO).append(";\n")
				.append("_H_BUFFERS_ADDR = ").append(_H_BUFFERS_ADDR).append(";\n")
				.append("mVertexSize = ").append(mVertexSize).append(",")
				.append(mVertexCoBuffer==null?0:mVertexCoBuffer.length).append(";\n")
				.append("mTextureSize = ").append(mTextureSize).append(",")
				.append(mTextureCoBuffer==null?0:mTextureCoBuffer.length).append(";\n")
				.append("mNormalSize = ").append(mNormalSize).append(",")
				.append(mNormalCoBuffer==null?0:mNormalCoBuffer.length).append(";\n")
				.append("mUseMtl = ").append(mUseMtl).append(";\n")
				.toString();
	};
}
