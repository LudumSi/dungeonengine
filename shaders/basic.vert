#version 140

in vec2 position;
in vec2 tex_coords;

uniform mat4 matrix;

out vec2 v_tex_coords;

void main() {
  v_tex_coords = tex_coords;
  gl_Position = vec4(position, 0.0, 1.0);
  gl_Position = matrix * gl_Position;
}
