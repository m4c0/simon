#ifndef BUILD_H
#define BUILD_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <unistd.h>
#elif _WIN32
#  define _CRT_SECURE_NO_WARNINGS
#  define _CRT_NONSTDC_NO_WARNINGS
#  include <process.h>
#endif

#include <assert.h>

static int compile_common();
static int link_exe();
static int compile_and_link_exe() {
  if (compile_common()) return 1;
  if (link_exe()) return 1;
  return 0;
}

int run(char ** args) {
  assert(args && args[0]);

#ifdef __APPLE__
  pid_t pid = fork();
  if (pid == 0) {
    execvp(args[0], args);
    abort();
  } else if (pid > 0) {
    int sl = 0;
    assert(0 <= waitpid(pid, &sl, 0));
    if (WIFEXITED(sl)) return WEXITSTATUS(sl);
  }
#elif _WIN32
  if (0 == _spawnvp(_P_WAIT, args[0], (const char * const *)args)) {
    return 0;
  }
#endif

  fprintf(stderr, "failed to run child process: %s\n", args[0]);
  return 1;
}
#define RUN(...) do { char * args[] = { __VA_ARGS__, 0 }; if (run(args)) return 1; } while (0)

#define CC1(src, o, ...) RUN("clang", "-Wall", __VA_ARGS__, "-o", o, "-c", src, "-include-pch", "pch.pch")
#define HDR(src, o, d) CC1(src, o, "-x", "c", "-D", d, CFLAGS)
#define CC(src, o) CC1(src, o, CFLAGS)

#define SHADER(src) RUN("glslang", "-V", src, "-o", RES_PATH "/" src ".spv")

static int compile_common() {
  //CC("src.c", "src.o");
  HDR("vlk.h", "vlk.o", "VLK_IMPL");
  return 0;
}
#undef ENG

static int shaders() {
  SHADER("shader.frag");
  SHADER("shader.vert");
  return 0;
}

#define OBJS "vlk.o"

#endif
