#version 300 es  
precision mediump float;  
 
uniform bool u_use_texture;  
uniform sampler2D u_sampler;

in vec2 f_texture; 
in vec4 f_color;
out vec4 fragColor;  
  
void main(){  
    fragColor = f_color;  
	if(u_use_texture){
		fragColor = vec4(1.0f,1.0f,1.0f,1.0f) * texture(u_sampler,f_texture);
	}
}