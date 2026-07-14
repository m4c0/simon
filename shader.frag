#version 450

layout(push_constant) uniform upc {
  vec2 aspect;
  float time;
  vec4 anims;
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

float anim_d(float a) {
  float d = pc.time - a;
  return 0.2 + 0.8 * 0.1 / abs(d);
}

vec4 c_sqr0(vec2 p, float anim) {
  float d = sd_sqr(p);
  float a = anim_d(anim);
  return vec4(a, 0, 0, 1 - step(0, d));
}
vec4 c_sqr1(vec2 p, float anim) {
  float d = sd_sqr(p);
  float a = anim_d(anim);
  return vec4(0, a, 0, 1 - step(0, d));
}
vec4 c_sqr2(vec2 p, float anim) {
  float d = sd_sqr(p);
  float a = anim_d(anim);
  return vec4(0, 0, a, 1 - step(0, d));
}
vec4 c_sqr3(vec2 p, float anim) {
  float d = sd_sqr(p);
  float a = anim_d(anim);
  return vec4(a, a, 0, 1 - step(0, d));
}

void main() {
  vec2 p = f_pos;

  vec4 c0 = c_sqr0(p - vec2(-0.5, -0.5), pc.anims.x);
  vec4 c1 = c_sqr1(p - vec2(+0.5, -0.5), pc.anims.y);
  vec4 c2 = c_sqr2(p - vec2(-0.5, +0.5), pc.anims.z);
  vec4 c3 = c_sqr3(p - vec2(+0.5, +0.5), pc.anims.w);

  colour = vec4(
      c0.rgb * c0.a +
      c1.rgb * c1.a +
      c2.rgb * c2.a +
      c3.rgb * c3.a,
      c0.a + c1.a + c2.a + c3.a);
}
