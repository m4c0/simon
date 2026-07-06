#ifndef GME_H
#define GME_H

void gme_reset();

#ifdef GME_IMPL

#include <stdlib.h>

static int gme_seq[102400];
static int gme_n;

void gme_reset() {
  gme_n = 1;
  gme_seq[0] = rand() % 4;
}

#endif
#endif
