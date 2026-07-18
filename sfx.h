#ifndef SFX_H
#define SFX_H

void sfx_init();
void sfx_filler(float * buf, unsigned sz);

void sfx_move();
void sfx_shuffle();
void sfx_win();

#ifdef SFX_IMPL

#include "gme.h"

#include <math.h>

static float sfx_rand_buf[1024];

static float sfx_env(float ssp) {
  if (ssp < 0.0) return 0;
  if (ssp < 0.1) return ssp / 0.1f;
  if (ssp < 1.0) return 1.0f - (ssp - 0.1f) / 0.9f;
  return 0;
}

static int sfx_smp = 0;
void sfx_filler(float * buf, unsigned sz) {
  const gme_state_t * gme = gme_state();

  for (unsigned i = 0; i < sz; ++i) {
    float t = (sfx_smp + i) / 44100.f;

    float v = 0;
    for (int i = 0; i < 4; i++) {
      float dt = t - gme->anims[i];
      v += sfx_env(dt) * sin(t * 440.f * 6.28f);
    }
    buf[i] = 0.25f * v;
  }
  sfx_smp += sz;
}

void sfx_init() {
  for (int i = 0; i < 1024; i++) {
    sfx_rand_buf[i] = (float)rand() / (float)RAND_MAX;
  }
}

#endif
#endif
