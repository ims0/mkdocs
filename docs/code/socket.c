/*********************************************************
    > File Name: socket.c
    > Author: ims
    > Created Time: 2020年10月07日 星期三 11时42分18秒
 *********************************************************/

#include <netinet/in.h>  /* IPPROTO_TCP */
#include <netinet/tcp.h> /* TCP_QUICKACK */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  int serveSockFd = socket(AF_INET, SOCK_STREAM, 0);
  int optval = 1;
  setsockopt(serveSockFd, IPPROTO_TCP, TCP_QUICKACK, &optval, sizeof(int));

  return 0;
}
