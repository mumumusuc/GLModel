#version 300 es  
  
layout(location = 0) in vec4 vPosition;  
layout(location = 1) in vec2 vTexture; 
layout(location = 2) in vec3 vNormal; 

uniform mat4 m_proj;
uniform mat4 m_model;
 
uniform bool u_use_light; 
uniform vec3 u_light_position;	//光源位置
uniform vec3 u_eye_position;	//相机位置

uniform vec4 u_ambient;			//环境光颜色
uniform vec3 u_light_color;		//光源颜色
uniform float u_Ns;						//反射指数
uniform vec3 u_Ka;						//环境光反射系数
uniform vec3 u_Kd;						//漫反射系数
uniform vec3 u_Ks;						//镜面反射系数
uniform float u_Ni;						//透光率
 
out vec4 f_color;
out vec2 f_texture;
  
void main() {  
	gl_Position = m_proj * (m_model * vPosition); 
	if(u_use_light){
		vec3 ECPosition = vec3(m_model * vPosition);
		vec3 N = vec3(normalize(m_model * vec4(vNormal,0.0)));
		vec3 L = normalize(u_light_position  - ECPosition);
		vec3 V = normalize(u_eye_position - ECPosition);
		vec3 H = normalize(V + L);
		vec3 diffuse = u_light_color * u_Kd * max(dot(N , L) , 0.0);
		vec3 specular = u_light_color * u_Ks * pow(max(dot(N , H) , 0.0) , u_Ns);
		f_color = vec4(clamp(u_Ka * (diffuse + specular) , 0.0 , 1.0) , u_Ni);
	}else{
    	f_color = vec4(1.0,1.0,1.0,0.5); 
    }   
    f_texture = vTexture;
} 