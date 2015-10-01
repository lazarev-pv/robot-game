#ifdef COMPILE_VERTEX
attribute vec3 a_position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;

void main(){
	gl_Position = vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
}
#else

varying vec2 v_texCoord;
uniform sampler2D u_light_tex;
uniform sampler2D u_layer_tex;
uniform vec3 u_ambientLight;

void main(void){
    vec4 l_c = texture2D(u_light_tex, v_texCoord);
    vec4 t_c = texture2D(u_layer_tex, v_texCoord);
    gl_FragColor = vec4(l_c.xyz+u_ambientLight,1.0)*t_c;
   // gl_FragColor = l_c;
}

#endif
