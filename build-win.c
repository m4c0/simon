//#define OPT "-gdwarf"
#define OPT "-O3"

#define CFLAGS OPT, "-IVulkan-Headers/include"
#define RES_PATH "app"
#include "build.h"

#define _CRT_SECURE_NO_WARNINGS
#include <sys/stat.h>
#include <assert.h>
#include <direct.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>

static int rc() {
  RUN("llvm-rc.exe", "/FO", "main.res", "main.rc");
  return 0;
}

static int pch() {
  RUN("clang", "-Wall", OPT, "-x", "c-header",
      "-IVulkan-Headers/include",
      "-D", "VK_USE_PLATFORM_WIN32_KHR",
      "-D", "VLK_USE_VOLK",
      "-o", "pch.pch", "pch.h");
  return 0;
}

static int link_exe() {
  RUN("clang", "-Wall", OPT,
      "-o", "app/puzzle.exe", "main.res",
      "app-win.o", "volk.o", OBJS,
      "-ladvapi32", "-lole32", "-lshell32", "-luser32");
  return 0;
}

int main(int argc, char ** argv) {
  _mkdir("app");

  if (pch()) return 1;

  CC("app-win.c", "app-win.o");
  CC("volk.c", "volk.o");
  if (rc()) return 1;
  if (compile_and_link_exe()) return 1;

  //if (cc("shots.c", "shots.o")) return 1;
  //if (link_shots_exe()) return 1;

  return 0;
}
