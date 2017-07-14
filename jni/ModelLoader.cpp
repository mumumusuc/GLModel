#include "ModelLoader.h"

using namespace std;
using namespace vmath;

//字符串分割函数
vector<string> split(string str,string pattern)
{
	string::size_type pos;
    vector<string> result;
    str += pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos == string::npos)
        	return result;
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}


char* readModelSrcFile(char* modelFile, AAssetManager *pAssetManager)
{
    AAsset *pAsset = NULL;
    char *pBuffer = NULL;
    off_t size = -1;
    int numByte = -1;

    if (NULL == pAssetManager){
        LOGE("pAssetManager is null!");
        return NULL;
    }

    pAsset = AAssetManager_open(pAssetManager, modelFile, AASSET_MODE_UNKNOWN);
    size = AAsset_getLength(pAsset);
    LOGI("after AAssetManager_open");
    pBuffer = (char *)malloc(size+1);
    pBuffer[size] = '\0';

    numByte = AAsset_read(pAsset, pBuffer, size);
    //LOGI("%s : [%s]", modelFile, pBuffer);
    AAsset_close(pAsset);

    return pBuffer;
}

//material
bool loadMtl(const char* mtlBuffer,vector<Material>& out_mtl){

	istringstream modelData(mtlBuffer);

	if( modelData == NULL ){
			LOGE(":( material load failed\n");
			return false;
		}
	Material temp_mtl;
	unsigned int mtl_index = 0;

	string line;
    istringstream data;
    while(getline(modelData,line)){
	  string start;
	  istringstream data(line);
	  data>>start;
	  if ( strcmp(start.c_str(), "newmtl" ) == 0 ){
		  if(mtl_index != 0){
			  out_mtl.push_back(temp_mtl);
		  }
		  __clearMaterial__(temp_mtl);
		  mtl_index++;
		  data>>temp_mtl.code;
		  LOGI("code = %d \n",temp_mtl.code);
	  }else if(strcmp(start.c_str(), "Ns" ) == 0){
		  data>>temp_mtl.Ns;
		  LOGI("Ns = %f \n",temp_mtl.Ns);
	  }else if(strcmp(start.c_str(), "Ka" ) == 0){
		  data>>temp_mtl.Ka[0];
		  data>>temp_mtl.Ka[1];
		  data>>temp_mtl.Ka[2];
		  LOGI("Ka = %f %f %f \n",temp_mtl.Ka[0],temp_mtl.Ka[1],temp_mtl.Ka[2]);
	  }else if(strcmp(start.c_str(), "Kd" ) == 0){
		  data>>temp_mtl.Kd[0];
		  data>>temp_mtl.Kd[1];
		  data>>temp_mtl.Kd[2];
		  LOGI("Kd = %f %f %f \n",temp_mtl.Kd[0],temp_mtl.Kd[1],temp_mtl.Kd[2]);
	  }else if(strcmp(start.c_str(), "Ks" ) == 0){
		  data>>temp_mtl.Ks[0];
		  data>>temp_mtl.Ks[1];
		  data>>temp_mtl.Ks[2];
		  LOGI("Ks = %f %f %f \n",temp_mtl.Ks[0],temp_mtl.Ks[1],temp_mtl.Ks[2]);
	  }else if(strcmp(start.c_str(), "map_Kd" ) == 0){
		  data>>temp_mtl.mtl_index;
		  LOGI("map_Kd = %d \n",temp_mtl.mtl_index);
	  }

  }
    out_mtl.push_back(temp_mtl);
    LOGI("out_mtl.size = %d",out_mtl.size());
    return true;
}

//model
bool loadOBJ(
		const char* modelBuffer,
        vector<vec3> & out_vertices,
        vector<vec2> & out_uvs,
        vector<vec3> & out_normals,
		vector<vec3> & out_mtlIndex){

        vector<unsigned int> vertexIndices, uvIndices, normalIndices;

        vector<vec3> temp_vertices;
        vector<vec2> temp_uvs;
        vector<vec3> temp_normals;
        vector<vec2> temp_mtls;

        long f_index = 0;

        istringstream modelData(modelBuffer);

        if( modelData == NULL ){
                LOGE(":( model load failed\n");
                return false;
        }

        string line;
        istringstream data;
        while(std::getline(modelData,line)){
                string start;
                istringstream data(line);
                data>>start;
                //LOGI("start with %s",start.c_str());
                if ( strcmp(start.c_str(), "v" ) == 0 ){
					vec3 vertex;
				    data>>vertex[0];
				    data>>vertex[1];
				    data>>vertex[2];
					temp_vertices.push_back(vertex);
					//LOGI("v = %f %f %f \n",vertex[0],vertex[1],vertex[2]);
                }
                else if (strcmp(start.c_str(), "vt" ) == 0 ){
					vec2 uv;
					data>>uv[0];
					data>>uv[1];
					//uv[1] = -uv[1]; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
					temp_uvs.push_back(uv);
					//LOGI("uv = %f %f \n",uv[0],uv[1]);
                }
                else if (strcmp(start.c_str(), "vn" ) == 0 ){
					vec3 normal;
					data>>normal[0];
					data>>normal[1];
					data>>normal[2];
					temp_normals.push_back(normal);
					//LOGI("vn = %f %f %f \n",normal[0],normal[1],normal[2]);
                }else if(strcmp(start.c_str(), "usemtl" ) == 0 ){
                	vec2 mtl;
                	data>>mtl[0];
                	mtl[1] = (float)f_index;
                	temp_mtls.push_back(mtl);
                	//LOGI("mtl_id =  %d , mtl_start = %d",mtl[0],mtl[1]);
                }
                else if (strcmp(start.c_str(), "f" ) == 0 ){
                	//添加mtl索引
                	//out_mtlIndex.push_back(temp_mtl);

					unsigned int vertex,uv,normal;
					vector<string> sp = split(line," ");
					for(int i=1;i<sp.size();i++){
						vector<string> st = split(sp[i],"/");
						//f只有顶点数据
						if(st.size() == 1){
							istringstream tv(sp[i]);
							tv>>vertex;
							vertexIndices.push_back(vertex);
							//LOGI("vertex = %d",vertex);
						}
						else{//有顶点、纹理和法向量
							istringstream tv(st[0]);
							tv>>vertex;
							//LOGI("vertexInd = %d",vertex);
							vertexIndices.push_back(vertex);
							if (*st[1].c_str() != NULL){
								istringstream tu(st[1]);
								tu>>uv;
								uvIndices.push_back(uv);
							}else{
								uvIndices.push_back(0);
							}
							istringstream tn(st[2]);
							tn>>normal;
							normalIndices.push_back(normal);
						}
					}
					f_index++ ;
                }
        }
        //重建材质索引
        vec3 mtl;
        for(int i = 0;i<temp_mtls.size()-1;i++){
        	mtl[0] = temp_mtls[i][0];
        	mtl[1] = temp_mtls[i][1];
        	mtl[2] = temp_mtls[i+1][1] - temp_mtls[i][1];
        	out_mtlIndex.push_back(mtl);
        	LOGI("mtl_id=%d , mtl_start=%d , mtl_size=%d",(int)mtl[0],(int)mtl[1],(int)mtl[2]);
        }
        // For each vertex of each triangle
        LOGI("vertex size = %d",vertexIndices.size());
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){

                unsigned int vertexIndex = vertexIndices[i];
                vec3 vertex = temp_vertices[ vertexIndex-1 ];
                out_vertices.push_back(vertex);

                if(normalIndices.size() > 0){
                	unsigned int normalIndex = normalIndices[i];
                	vec3 normal = temp_normals[ normalIndex-1 ];
                	out_normals.push_back(normal);
                }

                if(uvIndices.size() > 0){
					unsigned int uvIndex = uvIndices[i];
					vec2 uv = temp_uvs[ uvIndex-1 ];
					out_uvs.push_back(uv);
				}
        }

      vector<vec3>().swap(temp_vertices);
	  vector<unsigned int>().swap(vertexIndices);
	  vector<vec3>().swap(temp_normals);
	  vector<unsigned int>().swap(normalIndices);
	  vector<vec2>().swap(temp_uvs);
	  vector<unsigned int>().swap(uvIndices);

	  return true;
}
