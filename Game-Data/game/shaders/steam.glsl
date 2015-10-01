#ifdef COMPILE_VERTEX
uniform mat4 u_mvp;
uniform vec2 u_size;
uniform float u_time;
uniform vec2 u_dir;

attribute vec3 a_start;
attribute vec2 a_velocity;
varying vec3 v_var;


void main(){
    vec2 vel = u_dir * a_velocity.y*3.0;
    vec2 t = (u_dir * 2.0)* 2.0 / vel;
    vec2 accel = -vel / t ;
    vec2 shaderTime = mod(vec2(u_time,u_time),t);
    vec3 pos = a_start + vec3(vel*shaderTime+accel*shaderTime*shaderTime*0.5,0);

	gl_Position = u_mvp*vec4(pos*vec3(u_size,1.0),1.0);
	v_var.z = 1.0-shaderTime.x/(t.x) ;
	gl_PointSize = 20+5.0*(1.0-v_var.z);
	v_var.x=cos(shaderTime.x*3.5);
    v_var.y=sin(shaderTime.x*3.5);
}

#else

uniform sampler2D u_texture;
uniform vec4 u_color;
varying vec3 v_var;

void main(void){
    vec2 center = gl_PointCoord - vec2(0.5, 0.5);
    mat2 rotation = mat2(v_var.x, v_var.y,-v_var.y, v_var.x);
    center = rotation * center;
    //gl_FragColor = vec4(0,0,0,v_var.z*1.5)*texture2D( u_texture, center + vec2(0.5, 0.5));
    vec4 ppp = texture2D( u_texture, center + vec2(0.5, 0.5));
    gl_FragColor = vec4(u_color.xyz,v_var.z*u_color.w)*ppp;
}

#endif
