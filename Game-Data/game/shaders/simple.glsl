#ifdef COMPILE_VERTEX
attribute vec3 a_position;

uniform mat4 u_mvp_matrix;

void main(){
	gl_Position = u_mvp_matrix * vec4(a_position, 1.0);
	//gl_Position.z = 0.0f;
}

#else

uniform vec4 u_color;

void main(void){
	gl_FragColor = u_color; 
}

#endif
