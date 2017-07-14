#version 300 es  
  
layout(location = 0) in vec4 vPosition;  
layout(location = 1) in vec3 vNormal;  
layout(location = 2) in vec2 vTexture;
  
uniform mat4 projection_matrix;
uniform mat4 model_matrix;
 
uniform bool enableLight;
 
uniform vec3 LightPosition;

uniform vec3 Ld;
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Ns;

out vec3 f_color;
out vec2 TexCoord; 
  
void main(){  
    
    if(enableLight){
    
    	vec3 N = vec3(normalize(model_matrix * vec4(vNormal,0.0)));  
    	vec3 ECPosition = vec3(model_matrix * vPosition);  
    	vec3 L = normalize(LightPosition - ECPosition);  
    	vec3 V = normalize(vec3(-ECPosition));  
        vec3 H = normalize(V+L); 
    	
    	vec3 diffuse = Ld * max(dot(N , L) , 0.0);
    	vec3 specular = Ld * Ks * pow(max(dot(N , H) , 0.0) , Ns);
    	
    	//.. Ka + Kd*s*t + Ks*(r*v)^Ns
    	f_color =clamp((diffuse + specular) , 0.0 , 1.0); 
     }
     
    gl_Position = projection_matrix *(model_matrix * vPosition);  

    TexCoord = vTexture;
}  