/*********************************************************
    > File Name: brk.c
    > Author: ims
    > Created Time: Thu 29 Jul 2021 08:52:37 PM CST
 *********************************************************/
//https://blog.csdn.net/Apollon_krj/article/details/54565768
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 1024

int main() {

  void *p = sbrk(0);
  void *old = p;

  p = sbrk(MAX );
  if (p == (void *)-1) {
    perror("sbrk");
    exit(EXIT_FAILURE);
  }
  printf("old:%p\nret:%p\n", old, p);
  void *newp = sbrk(0);
  printf("cur:%p\n", newp);
  printf("pid:%d\n", getpid());
  sleep(1000);
  //sbrk(-MAX );
  return 0;
}
