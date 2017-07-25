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
uniform float u_Ns;				//高光系数
uniform float u_attenuation;	//光线的衰减系数
 
out vec4 f_color;
out vec2 f_texture;
  
void main() {  
	gl_Position = m_proj * (m_model * vPosition); 
	if(u_use_light){
		vec3 ECPosition = vec3(m_model * vPosition);
		vec3 N = vec3(normalize(m_model * vec4(vNormal,0.0)));
		vec3 L = normalize(u_light_position - ECPosition);
		vec3 V = normalize(ECPosition);
		vec3 H = normalize(V + L);
		vec3 diffuse = u_light_color * max(dot(N , L) , 0.0);
		vec3 specular = u_light_color * u_attenuation * pow(max(dot(N , H) , 0.0) , u_Ns);
		f_color = vec4(clamp((diffuse + specular) , 0.0 , 1.0) , 0.8);
	}else{
    	f_color = vec4(1.0,1.0,1.0,0.0); 
    }   
    f_texture = vTexture;
} 