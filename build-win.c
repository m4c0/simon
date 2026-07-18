#define OPT "-O3", "-gdwarf"

#define CFLAGS OPT, "-IVulkan-Headers/include"
#define RES_PATH "."
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
  RUN("clang", "-Wall", "-x", "c-header", CFLAGS,
      "-D", "VK_USE_PLATFORM_WIN32_KHR",
      "-D", "VLK_USE_VOLK",
      "-o", "pch.pch", "pch.h");
  return 0;
}

static int link_exe() {
  RUN("clang", "-Wall", OPT,
      "-o", "memory.exe", "main.res",
      "app-win.o", "volk.o", OBJS,
      "-ladvapi32", "-lole32", "-lshell32", "-luser32");
  return 0;
}

int main(int argc, char ** argv) {
  if (pch()) return 1;

  CC("app-win");
  CC("volk");
  if (shaders()) return 1;
  if (rc()) return 1;
  if (compile_and_link_exe()) return 1;

  return 0;
}
