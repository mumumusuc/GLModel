#version 300 es  
precision mediump float; 

in vec2 TexCoord; 
in vec3 f_color; 
out vec4 fragColor;  
  
uniform bool enableTexture;  
uniform sampler2D Tex1;
  
void main()  
{  
	if(enableTexture){
		vec4 texColor = texture(Tex1, TexCoord);  
    	fragColor = vec4(f_color,1.0) * texColor;
    }else{
    	fragColor = vec4(f_color,1.0);
    }  
}  