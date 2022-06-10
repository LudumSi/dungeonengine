#version 140

in vec2 v_tex_coords;
out vec4 frag;

//Alpha cutoff not to render
float cutoff = 1.0;

uniform sampler2D tex;

void main() {
  frag = texture(tex, v_tex_coords);

  if(frag.a < cutoff){
    discard;
  }
}
