#ifndef GME_H
#define GME_H

typedef struct gme_state_s {
  float clicks[4];
  float gameover;
  int playback;
  int hover;
} gme_state_t;

const gme_state_t * gme_state();

void gme_reset(void);
void gme_tick(void);

void gme_mouse_move(float px, float py);
void gme_mouse_down(void);

#ifdef GME_IMPL

#include "tim.h"

#define GME_SEQ_SIZE 102400

static int gme_seq[GME_SEQ_SIZE];
static int gme_streak;

static gme_state_t gme_st = {0};

static int gme_last_played;
static int gme_last_clicked;
static float gme_timer;

static int gme_click;

static void gme_set_timer(float t) {
  gme_timer = tim_now() + t;
}

void gme_reset(void) {
  srand(time(NULL));
  for (int i = 0; i < GME_SEQ_SIZE; i++) {
    gme_seq[i] = (int)(4.0 * (double)rand() / ((double)RAND_MAX + 1.0));
  }
  gme_streak = 1;
  gme_last_clicked = 0;
  gme_last_played = 0;
  gme_click = 0;
  gme_st.gameover = -1e10;
  gme_st.playback = 1;
  gme_st.hover = -1;

  gme_set_timer(0.5);

  for (int i = 0; i < 4; i++) gme_st.clicks[i] = -1e10;
}

void gme_tick(void) {
  int clicked = gme_click;
  gme_click = 0;

  if (gme_st.gameover > 0) return;

  if (gme_timer > tim_now()) return;

  if (gme_st.playback) {
    if (gme_last_played >= gme_streak) {
      gme_last_clicked = 0;
      gme_st.playback = 0;
      return;
    }

    int n = gme_seq[gme_last_played];
    gme_st.clicks[n] = tim_now();
    gme_last_played++;
    gme_set_timer(1.0);
    return;
  } 

  if (gme_st.hover == -1) return;

  if (!clicked) return;

  gme_st.clicks[gme_st.hover] = tim_now();

  if (gme_seq[gme_last_clicked] != gme_st.hover) {
    gme_st.gameover = tim_now();
    return;
  }

  gme_last_clicked++;
  if (gme_last_clicked < gme_streak) return;

  gme_streak++;
  gme_st.hover = -1;
  gme_st.playback = 1;
  gme_last_played = 0;
  gme_set_timer(1.0);
}

void gme_mouse_move(float px, float py) {
  gme_st.hover = -1;
  if (-0.9 < px && px < -0.1) {
    if (-0.9 < py && py < -0.1) {
      gme_st.hover = 0;
    } else if (0.9 > py && py > 0.1) {
      gme_st.hover = 2;
    }
  } else if (0.9 > px && px > 0.1) {
    if (-0.9 < py && py < -0.1) {
      gme_st.hover = 1;
    } else if (0.9 > py && py > 0.1) {
      gme_st.hover = 3;
    }
  }
}

void gme_mouse_down() {
  gme_click = 1;
}

const gme_state_t * gme_state() { return &gme_st; }

#endif
#endif
