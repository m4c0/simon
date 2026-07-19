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

float anim_d(float a) {
  float d = pc.time - a;
  return 0.2 + 0.8 * 0.1 / abs(d);
}

vec4 c_sqr0(vec2 p, float anim, bool hover) {
  float d = sd_sqr(p);
  float a = anim_d(anim) + (hover ? 0.2 : 0.0);
  return vec4(a, 0, 0, 1 - step(0, d));
}
vec4 c_sqr1(vec2 p, float anim, bool hover) {
  float d = sd_sqr(p);
  float a = anim_d(anim) + (hover ? 0.2 : 0.0);
  return vec4(0, a, 0, 1 - step(0, d));
}
vec4 c_sqr2(vec2 p, float anim, bool hover) {
  float d = sd_sqr(p);
  float a = anim_d(anim) + (hover ? 0.2 : 0.0);
  return vec4(0, 0, a, 1 - step(0, d));
}
vec4 c_sqr3(vec2 p, float anim, bool hover) {
  float d = sd_sqr(p);
  float a = anim_d(anim) + (hover ? 0.2 : 0.0);
  return vec4(a, a, 0, 1 - step(0, d));
}

float gameover() {
  float dt = pc.time - pc.gameover;
  return step(0, dt) * (1 - step(1, dt));
}

void main() {
  vec2 p = f_pos;
  
  vec3 bg = pc.playback > 0 ? vec3(0.1) : vec3(0.2);

  float go = gameover();
  vec4 a = mix(pc.anims, vec4(pc.gameover), go);

  vec4 c0 = c_sqr0(p - vec2(-0.5, -0.5), a.x, pc.hover == 0);
  vec4 c1 = c_sqr1(p - vec2(+0.5, -0.5), a.y, pc.hover == 1);
  vec4 c2 = c_sqr2(p - vec2(-0.5, +0.5), a.z, pc.hover == 2);
  vec4 c3 = c_sqr3(p - vec2(+0.5, +0.5), a.w, pc.hover == 3);
  vec4 c = vec4(
      c0.rgb * c0.a +
      c1.rgb * c1.a +
      c2.rgb * c2.a +
      c3.rgb * c3.a,
      c0.a + c1.a + c2.a + c3.a);

  colour = vec4(mix(bg, c.rgb, c.a), 1);
}
