#ifdef COMPILE_VERTEX
attribute vec3 a_position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;

uniform mat4 u_mvp_matrix;

void main(){
	gl_Position = u_mvp_matrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
}
#else
const float blurSize = 10.0/512.0;

//precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D s_texture;

void main(void){

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them

   vec4 sum = vec4(0.0);
   sum += texture2D(s_texture, vec2(v_texCoord.x - 4.0*blurSize, v_texCoord.y)) * 0.05;
   sum += texture2D(s_texture, vec2(v_texCoord.x - 3.0*blurSize, v_texCoord.y)) * 0.09;
   sum += texture2D(s_texture, vec2(v_texCoord.x - 2.0*blurSize, v_texCoord.y)) * 0.12;
   sum += texture2D(s_texture, vec2(v_texCoord.x - blurSize, v_texCoord.y)) * 0.15;
   sum += texture2D(s_texture, vec2(v_texCoord.x, v_texCoord.y)) * 0.16;
   sum += texture2D(s_texture, vec2(v_texCoord.x + blurSize, v_texCoord.y)) * 0.15;
   sum += texture2D(s_texture, vec2(v_texCoord.x + 2.0*blurSize, v_texCoord.y)) * 0.12;
   sum += texture2D(s_texture, vec2(v_texCoord.x + 3.0*blurSize, v_texCoord.y)) * 0.09;
   sum += texture2D(s_texture, vec2(v_texCoord.x + 4.0*blurSize, v_texCoord.y)) * 0.05;
 
   gl_FragColor = sum;


    //gl_FragColor = texture2D(s_texture, v_texCoord);
}

#endif
