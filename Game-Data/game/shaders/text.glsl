#ifdef COMPILE_VERTEX

attribute vec4 a_position;
attribute vec2 a_texCoord;

uniform mat4 u_mvp_matrix;
varying vec2 v_texCoord;

void main(){
	gl_Position = u_mvp_matrix*a_position;
	v_texCoord = a_texCoord;
}

#else

varying vec2 v_texCoord;
uniform sampler2D s_texture;
uniform vec4 u_textColor;

void main(void){
	gl_FragColor = vec4(u_textColor.xyz, texture2D(s_texture, v_texCoord).a*u_textColor.w)/*+vec4(1,0,0,0.5)*/;
}

#endif
