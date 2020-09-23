#include <mcheck.h>
#include <stdlib.h>
#include <stdio.h>

// int setenv(const char *name, const char *value, int overwrite);
int main(void) {
  setenv("MALLOC_TRACE", "./mtrace_output", 1);
  mtrace();
  int *p1 = (int *)malloc(10);
  int *p2 = (int *)malloc(10);
  printf("p1:%p\n",p1);
  printf("p2:%p\n",p2);
  free(p1);
  muntrace();
  return 0;
}
