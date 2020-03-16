#include <stdio.h>
#include <stdlib.h>
#include "leakfinder.h"

void foo() {
  char *p= malloc(1024);
  free(p);
}

void leak(void){
  malloc(1024);
  malloc(1024);
  malloc(1024);

  foo();
}

int main(int argc, char **argv) {

  char *p= malloc(1024);

  leak();

  free(p);

  show_result();
  return 0;
}

