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
static int gme_streak;

static int gme_replaying;
static int gme_last_played;
static int gme_last_clicked;
static float gme_timer;

static int gme_hover;
static int gme_click;

void gme_set_timer(float t) {
  gme_timer = tim_now() + t;
}

void gme_reset(float * anims) {
  srand(time(NULL));
  for (int i = 0; i < GME_SEQ_SIZE; i++) {
    gme_seq[i] = (int)(4.0 * (double)rand() / ((double)RAND_MAX + 1.0));
  }
  gme_streak = 1;
  gme_last_clicked = 0;
  gme_last_played = 0;
  gme_hover = -1;
  gme_click = 0;
  gme_replaying = 1;

  gme_set_timer(0.5);

  for (int i = 0; i < 4; i++) anims[i] = -1e10;
}

// TODO: add sounds
void gme_tick(float * anims) {
  int clicked = gme_click;
  gme_click = 0;

  if (gme_timer > tim_now()) return;

  if (gme_replaying) {
    if (gme_last_played >= gme_streak) {
      gme_last_clicked = 0;
      gme_replaying = 0;
      return;
    }

    anims[gme_seq[gme_last_played]] = tim_now();
    gme_last_played++;
    gme_set_timer(1.0);
    return;
  } 

  if (gme_hover == -1) return;

  anims[gme_hover] = tim_now() - 0.15;

  if (!clicked) return;

  anims[gme_hover] = tim_now();

  if (gme_seq[gme_last_clicked] != gme_hover) {
    // TODO: game over
    gme_reset(anims);
    gme_set_timer(2);
    return;
  }

  gme_last_clicked++;
  if (gme_last_clicked < gme_streak) return;

  // TODO: add a delay after clicking
  // TODO: add a larger delay after completing

  gme_streak++;
  gme_hover = -1;
  gme_replaying = 1;
  gme_last_played = 0;
  gme_set_timer(1.0);
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
}

#endif
#endif
