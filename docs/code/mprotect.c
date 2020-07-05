#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

static void handler(int sig, siginfo_t *si, void *unused) {
  /* Note: calling printf() from a signal handler is not safe
     (and should not be done in production programs), since
     printf() is not async-signal-safe; see signal-safety(7).
     Nevertheless, we use printf() here as a simple way of
     showing that the handler was called. */

  printf("Got SIGSEGV at address: 0x%lx\n", (long)si->si_addr);
  exit(EXIT_FAILURE);
}

int mprotect(void *addr, size_t len, int prot);

int main(int argc, char **argv) {

  /* handle SIGSEGV signal */
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = handler;
  if (sigaction(SIGSEGV, &sa, NULL) == -1)
    handle_error("sigaction");

  /* 获取操作系统一个页的大小, 一般是 4KB == 4096 */
  int sysMemPageSize = sysconf(_SC_PAGE_SIZE);
  if (sysMemPageSize == -1)
    handle_error("sysconf");

  /* Allocate a buffer aligned on a page boundary;
          initial protection is PROT_READ | PROT_WRITE */

  char *buffer = (char *)memalign(sysMemPageSize, 2 * sysMemPageSize);
  if (buffer == NULL)
    handle_error("memalign");

  printf("Start of region:  %p\n", buffer);
  /* 设置属性前可写*/
  for (int i = 0; i < 2 * sysMemPageSize; ++i) {
    buffer[i] = i;
  }
  /*mprotect要求所保护的内存需要满足：
   * 1）被保护内存的首地址是一个内存页的首地址；
   * 2）保护的长度是一个内存页的整数倍*/
  if (mprotect(buffer + sysMemPageSize, sysMemPageSize, PROT_READ) == -1)
    handle_error("mprotect");

  for (char *p = buffer;;)
    *(p++) = 'a';

  printf("Loop completed\n"); /* Should never happen */

  free(buffer);
  exit(EXIT_SUCCESS);
  return 0;
}
