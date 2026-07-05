#version 450

layout(location=0) in vec2 f_pos;
layout(location=0) out vec4 colour;

float sd_sqr(vec2 p) {
  vec2 d = abs(p) - 0.5;
  return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

void main() {
  float d = sd_sqr(f_pos);

  colour = mix(vec4(1), vec4(0), step(0, d));
}
