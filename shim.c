#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include  <unistd.h>
#include <dlfcn.h>

int rand(){
  int (*orig_rand) (void) = dlsym(RTLD_NEXT, "rand");
  return orig_rand() % 100;
  //return 42;
}
