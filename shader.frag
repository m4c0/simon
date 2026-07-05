#version 450

layout(location=0) in vec2 f_pos;
layout(location=0) out vec4 colour;

float sd_box(vec2 p, vec2 b) {
  vec2 d = abs(p) - b;
  return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

void main() {
  vec2 p = f_pos;
  p = abs(p) - vec2(0.5);
  float d = sd_box(p, vec2(0.3)) - 0.05;

  vec3 c = mix(
      mix(vec3(1, 0, 0), vec3(0, 1, 0), step(0, f_pos.x)),
      mix(vec3(0, 0, 1), vec3(1, 1, 0), step(0, f_pos.x)),
      step(0, f_pos.y));

  colour = mix(vec4(c, 1), vec4(0), step(0, d));
}
