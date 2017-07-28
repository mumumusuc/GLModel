package com.mumu.glmodel.model;

public class ModelStruct {
	public int _H_vao = 0;
	// 顶点
	public int _H_vertex_co = 0;
	// 纹理
	public int _H_texture = 0;
	public int _H_texture_co = 0;
	public int _texture_unit = 0;
	// 法线
	public int _H_normal_co = 0;
	
	public ModelCoordinate mCoordinate;
	public ModelMtl mMtl;
	
	public ModelStruct(ModelCoordinate c,ModelMtl m){
		mCoordinate = c;
		mMtl = m;
	}
	
	@Override
	public String toString(){
		return new StringBuilder()
				.append("_H_vertex_co = ").append(_H_vertex_co).append(";\n")
				.append("_H_texture_co = ").append(_H_texture_co).append(";\n")
				.append("_H_texture = ").append(_H_texture).append(";\n")
				.append("_H_texture_unit = ").append(_texture_unit).append(";\n")
				.append("_H_normal_co = ").append(_H_normal_co).append(";\n")
				.toString();
	}
}
