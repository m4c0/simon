#version 450

layout(push_constant) uniform upc {
  vec2  aspect;
  float playback;
  float gameover;
  vec4  anims;
  float time;
  uint  hover;
} pc;

layout(location=0) in vec2 f_pos;
layout(location=0) out vec4 colour;

float sd_box(vec2 p, vec2 b) {
  vec2 d = abs(p) - b;
  return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float sd_sqr(vec2 p) {
  return sd_box(p, vec2(0.3)) - 0.05;
}

float sd_sqr_fin(vec2 p, float anim) {
  float t = pc.time - anim;

  float damp = sin(t * 6.28 * 1.5) * (1 - t) * step(0, t) * (1 - step(1, t));

  float ang = sin(anim * 10) * damp * 0.05;
  p = mat2(
    cos(ang), sin(ang),
    -sin(ang), cos(ang)
  ) * p;
  return sd_sqr(p) + damp * 0.05;
}

vec4 c_sqr(vec2 p, vec3 c, float anim, bool hover) {
  float d = sd_sqr_fin(p, anim);

  float h = hover ? 0.4 : 0.2;
  float v = h + 0.2 + 0.8 * 0.1 / abs(pc.time - anim);
  c = mix(c * v, vec3(0), step(0, d));

  float a = -tanh(d * 4.0 - 1.5) * 0.5 + 0.5;
  return vec4(c, a);
}

float gameover() {
  float dt = pc.time - pc.gameover;
  return step(0, dt) * (1 - step(10, dt));
}

float sd_board(vec2 p, vec4 a) {
  float d0 = sd_sqr_fin(p - vec2(-0.5, -0.5), a.x);
  float d1 = sd_sqr_fin(p - vec2(+0.5, -0.5), a.y);
  float d2 = sd_sqr_fin(p - vec2(-0.5, +0.5), a.z);
  float d3 = sd_sqr_fin(p - vec2(+0.5, +0.5), a.w);
  return min(d0, min(d1, min(d2, d3)));
}

// Based on this shader by David Hoskins: https://www.shadertoy.com/view/MdlXz8
vec3 c_bg() {
  float time = pc.time * 0.05;
  vec2 p = f_pos * 2.0 - 350.0;

  vec2 i = p;
  float c = 1;
  for (int n = 0; n < 5; n++) {
    float t = time * (1.0 - (3.5 / float(n + 1)));
    i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
    c += 1.0 / length(vec2(p.x / (sin(i.x + t) / 0.005), p.y / (cos(i.y + t) / 0.005)));
  }
  c = 1.17 - pow(c / 5.0, 1.4);

  vec3 col = vec3(pow(abs(c), 2.0));
  return clamp(col - vec3(0.3, 0.35, 0.5), 0, 1) * 0.2;
}

// Variant of Inigo Quilez' segment
float sd_segment(vec2 p, vec2 a, vec2 ba) {
  vec2 pa = p - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  return length(pa - ba * h);
}
// Variant of Inigo Quilez' arc
float sd_arc(vec2 p, float a, float r) {
  // sc is the sin/cos of the arc's aperture
  vec2 sc = vec2(sin(a), cos(a));
  p.x = abs(p.x);
  return (sc.y * p.x > sc.x * p.y)
    ? length(p - sc * r)
    : abs(length(p) - r);
}
float sd_arrow(vec2 p) {
  float t = pc.time;
  p = mat2(cos(t), -sin(t), sin(t), cos(t)) * p;

  const float r = 0.1;
  const float a = 2.6; // Apperture angle
  float d = sd_arc(p, a, r);

  // Arrow head
  p = mat2(cos(a), -sin(a), sin(a), cos(a)) * p;
  d = min(d, sd_segment(p, vec2(0.0, r), vec2(0.2,  0.2) * r));
  d = min(d, sd_segment(p, vec2(0.0, r), vec2(0.2, -0.4) * 0.7 * r));
  return d;
}

void main() {
  vec2 p = f_pos;
  
  // TODO: good vibe messages on player turn ("Be awesome!" etc)
  vec3 bg = pc.playback > 0 ? vec3(0.1) : vec3(0.2);

  float go = gameover();
  vec4 a = mix(pc.anims, vec4(pc.gameover), go);

  vec4 c0 = c_sqr(p - vec2(-0.5, -0.5), vec3(0, 0, 1), a.x, pc.hover == 0);
  vec4 c1 = c_sqr(p - vec2(+0.5, -0.5), vec3(1, 0, 1), a.y, pc.hover == 1);
  vec4 c2 = c_sqr(p - vec2(-0.5, +0.5), vec3(1, 0, 0), a.z, pc.hover == 2);
  vec4 c3 = c_sqr(p - vec2(+0.5, +0.5), vec3(1, 1, 0), a.w, pc.hover == 3);

  vec3 c = mix(bg,
      c0.rgb * c0.a +
      c1.rgb * c1.a +
      c2.rgb * c2.a +
      c3.rgb * c3.a,
      c0.a + c1.a + c2.a + c3.a);

  float d = sd_board(p, a);
  c = mix(c, c_bg(), smoothstep(0, 0.015, d));

  d = sd_arrow(p) + (1 - go);
  c = mix(vec3(0.0), c, smoothstep(0.01, 0.03, d));
  c = mix(vec3(0.7), c, smoothstep(0.01, 0.013, d));

  colour = vec4(c, 1);
}
