#ifndef GME_H
#define GME_H

void gme_reset(float * anims);
void gme_tick(float * anims);

void gme_mouse_move(float px, float py);
void gme_mouse_down();

#ifdef GME_IMPL

#include <stdlib.h>
#include "tim.h"

#define GME_SEQ_SIZE 102400

static int gme_seq[GME_SEQ_SIZE];
static int gme_n;
static int gme_last_played;
static float gme_playback;

static int gme_hover;
static int gme_click;

void gme_reset(float * anims) {
  srand(time(NULL));
  for (int i = 0; i < GME_SEQ_SIZE; i++) {
    gme_seq[i] = (int)(4.0 * (double)rand() / ((double)RAND_MAX + 1.0));
  }
  gme_n = 1;
  gme_playback = tim_now() - (1 - 0.5); // start first in 0.5ms
  gme_last_played = 0;
  gme_hover = -1;

  for (int i = 0; i < 4; i++) anims[i] = -1e10;
}

void gme_tick(float * anims) {
  int clicked = gme_click;
  gme_click = 0;

  if (gme_last_played >= gme_n) {
    if (gme_hover >= 0) anims[gme_hover] = tim_now();
    return;
  }

  float dt = tim_now() - gme_playback;
  if (dt < 1) return;

  anims[gme_seq[gme_last_played]] = tim_now();
  gme_last_played++;
  gme_playback = tim_now();
}

void gme_mouse_move(float px, float py) {
  gme_hover = -1;
  if (-0.9 < px && px < -0.1) {
    if (-0.9 < py && py < -0.1) {
      gme_hover = 0;
    } else if (0.9 > py && py > 0.1) {
      gme_hover = 2;
    }
  } else if (0.9 > px && px > 0.1) {
    if (-0.9 < py && py < -0.1) {
      gme_hover = 1;
    } else if (0.9 > py && py > 0.1) {
      gme_hover = 3;
    }
  }
}

void gme_mouse_down() {
  gme_click = 1;
  // if (gme_last_played < gme_n) return;
  // if (gme_hover == -1) return;
  // gme_n++;
  // gme_playback = tim_now() - (1 - 0.5); // start first in 0.5ms
  // gme_hover = -1;
}

#endif
#endif
