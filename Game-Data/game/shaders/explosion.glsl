#ifdef COMPILE_VERTEX

uniform mat4 u_mvp;
uniform vec2 u_size;
uniform float u_time;

attribute float a_lifetime;
attribute vec3 a_startPosition;
attribute vec3 a_endPosition;
varying vec3 v_var;

void main(){
    float shaderTime = mod(u_time,1.0);
  // float shaderTime = mod(u_time,a_lifetime+10);
  //  if ( shaderTime <= a_lifetime ){


        //my varr
        //vec3 localPos = a_startPosition+((a_endPosition-a_startPosition)/a_lifetime)*shaderTime;
        //book var
        vec3 localPos = a_startPosition + a_endPosition*shaderTime*2.0;

        gl_Position = u_mvp*vec4(localPos*vec3(u_size,0.0f), 1.0);
 /*   }else{
        gl_Position = vec4( -1000, -1000, 0, 0 );
    }*/

    v_var.z = clamp (shaderTime/a_lifetime, 0.0, 1.0 );
	gl_PointSize = ( v_var.z/**v_var.z */) * 40.0+40;
}

#else


uniform sampler2D u_texture;
uniform vec4 u_color;
varying vec3 v_var;

void main(void){
    /*vec2 center = gl_PointCoord - vec2(0.5, 0.5);
    mat2 rotation = mat2(v_var.x, v_var.y,-v_var.y, v_var.x);
    center = rotation * center;
    gl_FragColor = u_color * texture2D( u_texture, center + vec2(0.5, 0.5));
   */ gl_FragColor = u_color * texture2D( u_texture, gl_PointCoord);
    gl_FragColor.a *= (1.0-v_var.z);
}


#endif
