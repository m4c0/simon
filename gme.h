#ifndef GME_H
#define GME_H

void gme_reset(float * anims);
void gme_tick(float * anims);

#ifdef GME_IMPL

#include <stdlib.h>
#include "tim.h"

#define GME_SEQ_SIZE 102400

static int gme_seq[GME_SEQ_SIZE];
static int gme_n;
static int gme_last_played;
static float gme_playback;

void gme_reset(float * anims) {
  srand(time(NULL));
  for (int i = 0; i < GME_SEQ_SIZE; i++) {
    gme_seq[i] = (int)(4.0 * (double)rand() / ((double)RAND_MAX + 1.0));
  }
  gme_n = 1;
  gme_playback = tim_now() - (1 - 0.5); // start first in 0.5ms
  gme_last_played = 0;

  for (int i = 0; i < 4; i++) anims[i] = -1e10;
}

void gme_tick(float * anims) {
  if (gme_last_played >= gme_n) return;

  float dt = tim_now() - gme_playback;
  if (dt < 1) return;

  int n = gme_seq[gme_last_played];
  for (int i = 0; i < 4; i++) anims[i] = -1e10;
  anims[n] = tim_now();
  gme_last_played++;
  gme_playback = tim_now();
}

#endif
#endif
