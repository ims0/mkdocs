/*
 * 进程分配内存有两种方式，分别由两个系统调用完成：brk和mmap（不考虑共享内存）。
  1、brk是将数据段(.data)的最高地址指针_edata往高地址推 
  2、mmap是在进程的虚拟地址空间中（堆和栈中间，称为文件映射区域的地方）找一块空闲的虚拟内存。
  3. malloc小于128k的内存，使用brk分配内存
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#define SIZE 1024 * 100

int main() {
  //在进程启动时候，加入以下两行代码：
  mallopt(M_MMAP_MAX, 0); // 禁止malloc调用mmap分配内存
  mallopt(M_TRIM_THRESHOLD, -1); // 禁止内存紧缩

  printf("pid: %d\n", getpid());
  void *p = NULL;
  while (1) {
    printf("cur:%p\n", sbrk(0));
    p = malloc(SIZE);
    sleep(5);
  }
  return 0;
}
