#ifdef COMPILE_VERTEX
uniform mat4 u_mvp;
attribute vec4 a_atr;

varying float v_gradient;
void main(){
	gl_Position = u_mvp*vec4(a_atr.xyz,1.0);
	v_gradient=a_atr.w;
}

#else

varying float v_gradient;
void main(void){
    float c = 1.0-abs(v_gradient);
    gl_FragColor = vec4(c,c,1,c);
}

#endif
