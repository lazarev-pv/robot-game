#ifdef COMPILE_VERTEX
uniform mat4 u_mvp;
uniform vec2 u_size;
uniform float u_time;

attribute vec3 a_start;
attribute vec2 a_velocity;
attribute float a_lifetime;

void main(){
    float shaderTime = mod(u_time,a_lifetime*min(u_size.x,u_size.y));
	gl_Position = u_mvp*vec4(a_start*vec3(u_size,1.0)+vec3(a_velocity*shaderTime,0.0),1.0);
	//gl_PointSize = 5.0;
}

#else

uniform vec4 u_color;
//uniform sampler2D u_texture;

void main(void){
	gl_FragColor = vec4(u_color);/**texture2D( u_texture, gl_PointCoord );*/
}

#endif
