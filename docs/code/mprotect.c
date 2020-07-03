#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int mprotect(void *addr, size_t len, int prot);

int main() {
  /* 获取操作系统一个页的大小, 一般是 4KB == 4096 */
  int sysMemPageSize = sysconf(_SC_PAGE_SIZE);
  if (sysMemPageSize == -1) {
    perror("sysconf fail");
    return -1;
  }
  printf("Page Size is:%d Byte\n", sysMemPageSize);

  /* 按页对齐来申请一页内存, pProtect会是一个可以被页(0x1000 ==
   * 4096)整除的地址 */
  int *pProtect;
  /* posix_memalign 函数不仅分配内存，还将分配的内存按要求的字节长度对齐
   * 返回失败不会设置系统的errno, 不能用perror输出错误 */
  int ret = posix_memalign((void **)&pProtect, sysMemPageSize, sysMemPageSize);
  if (ret != 0) {
    printf("posix_memalign fail, ret %u\r\n", ret);
    return -1;
  }

  printf("posix_memalign mem %p\r\n", pProtect);

  /*mprotect要求所保护的内存需要满足：
   * 1）被保护内存的首地址是一个内存页的首地址；
   * 2）保护的长度是一个内存页的整数倍*/
  ret = mprotect(pProtect, sysMemPageSize, PROT_READ);
  if (ret == -1) {
    perror("mprotect");
    return -1;
  }
  //*pProtect = 1;

  free(pProtect);
  return 0;
}
