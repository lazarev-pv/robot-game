#ifdef COMPILE_VERTEX
attribute vec3 a_position;
attribute vec2 a_texCoord;
//attribute vec3 a_norm;
varying vec2 v_texCoord;

uniform mat4 u_mvp_matrix;

void main(){
	gl_Position = u_mvp_matrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
}
#else

varying vec2 v_texCoord;
uniform sampler2D s_texture;
uniform vec4 u_ambientLight;

void main(void){
	gl_FragColor = clamp(u_ambientLight*texture2D(s_texture, v_texCoord),0.0,1.0);
}

#endif
