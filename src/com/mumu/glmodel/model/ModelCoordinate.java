package com.mumu.glmodel.model;

public class ModelCoordinate {
	public String mUseMtl;

	public int mVertexSize = 0;
	public float[] mVertexCoBuffer;

	public int mTextureSize = 0;
	public float[] mTextureCoBuffer;

	public int mNormalSize = 0;
	public float[] mNormalCoBuffer;
	
	@Override public String toString(){
		return new StringBuilder()
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
