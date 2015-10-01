#ifdef COMPILE_VERTEX
uniform mat4 u_mvp;
uniform vec2 u_size;
uniform float u_time;
uniform vec2 u_dir;

attribute vec3 a_start;
attribute vec2 a_velocity;
attribute float a_lifetime;
varying vec3 v_var;


void main(){
    float shaderTime = mod(u_time,a_lifetime);
	gl_Position = u_mvp*vec4((a_start+vec3(vec2(1.0, a_velocity.y)*u_dir*shaderTime,0.0))*vec3(u_size,1.0),1.0);
	v_var.z = 1.0-shaderTime/(a_lifetime);
	gl_PointSize = 25.0 - 25.0*(1.0-v_var.z);
	v_var.x=cos(shaderTime*3.5);
    v_var.y=sin(shaderTime*3.5);
}

#else

uniform sampler2D u_texture;
varying vec3 v_var;

void main(void){
    vec2 center = gl_PointCoord - vec2(0.5, 0.5);
    mat2 rotation = mat2(v_var.x, v_var.y,-v_var.y, v_var.x);
    center = rotation * center;
    gl_FragColor = vec4(1,v_var.z*0.5,0,v_var.z*1.5)*texture2D( u_texture, center + vec2(0.5, 0.5));
}

#endif
