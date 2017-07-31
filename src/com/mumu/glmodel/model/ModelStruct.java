package com.mumu.glmodel.model;

public class ModelStruct {
	
	public ModelCoordinate mCoordinate;
	public ModelMtl mMtl;
	
	public ModelStruct(){}
	
	public ModelStruct(ModelCoordinate c,ModelMtl m){
		mCoordinate = c;
		mMtl = m;
	}
	
	@Override
	public String toString(){
		return new StringBuilder()
				.append("mCoordinate = ").append(mCoordinate == null?null:mCoordinate.toString()).append(";\n")
				.append("mMtl = ").append(mMtl == null?null:mMtl.toString()).append(";\n")
				.toString();
	}
	
	public long getMemeryUsage(){
		long total = 0;
		if(mCoordinate != null)
			total += mCoordinate.getMemeryUsage();
		if(mMtl != null)
			total += mMtl.getMemeryUsage();
		return total;
	}
}
