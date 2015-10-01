#ifdef COMPILE_VERTEX
uniform mat4 u_mvp;
uniform vec2 u_size;
uniform float u_time;

attribute vec3 a_start;
attribute vec2 a_velocity;
attribute float a_lifetime;

void main(){

    float time = mod(u_time,a_lifetime);
    vec3 timeOffset = vec3(mod(floor(u_time/a_lifetime),10.0)/10.0,0.0,0.0);
    vec2 offset = a_velocity*time+vec2(0.0,-4.9)*time*time;
	gl_Position = u_mvp*vec4((a_start+timeOffset)*vec3(u_size,1.0)+vec3(offset,0.0),1.0);
    gl_PointSize = 2.0;
}

#else

void main(void){
	gl_FragColor = vec4(1,1,1,0.4); 
}

#endif
