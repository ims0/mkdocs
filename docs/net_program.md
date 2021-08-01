## SOCKET
### socket大致介绍
  由于现在是面向对象的编程，一些计算机行业的大神通过抽象的理念，在现实中通过反复的理论或者实际的推导，提出了抽象的一些通信协议，基于tcp/ip协议，提出大致的构想，一些泛型的程序大牛在这个协议的基础上，将这些抽象化的理念接口化，针对协议提出的每个理念，专门的编写制定的接口，与其协议一一对应，形成了现在的socket标准规范，然后将其接口封装成可以调用的接口，供开发者使用
![avatar](tcp_ip_pic/socket_site.png)

### net function
#### socket
```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
```

+ domain:协议族， AF_INET/AF_INET6
+ type:类型， SOCK_STREAM/SOCK_DGRAM
+ protocol：协议,0自动选择，/etc/protocols

#### bind

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

+ 当我们调用socket创建一个socket时，返回的socket描述字它存在于协议族（address family，AF_XXX）空间中，但没有一个具体的地址。如果想要给它赋值一个地址，就必须调用bind()函数，否则就当调用connect()、listen()时系统会自动随机分配一个端口。

+ addr：一个const struct sockaddr *指针，指向要绑定给sockfd的协议地址。这个地址结构根据地址创建socket时的地址协议族的不同而不同
+ addrlen：对应的是地址的长度。

+ 通常服务器在启动的时候都会绑定一个众所周知的地址（如ip地址+端口号），用于提供服务，客户就可以通过它来接连服务器；而客户端就不用指定，有系统自动分配一个端口号和自身的ip地址组合。这就是为什么通常服务器端在listen之前会调用bind()，而客户端就不会调用，而是在connect()时由系统随机生成一个。



#### listen 

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```
listen函数的第一个参数即为要监听的socket描述字，第二个参数为相应socket可以排队的最大连接个数。socket()函数创建的socket默认是一个主动类型的，listen函数将socket变为被动类型的，等待客户的连接请求。

#### connect
```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
connect函数的第一个参数即为客户端的socket描述字，第二参数为服务器的socket地址，第三个参数为socket地址的长度。客户端通过调用connect函数来建立与TCP服务器的连接。

#### accept
```c
#include <sys/types.h>
#include <sys/socket.h>

int connect(int socket, const struct sockaddr *address, socklen_t address_len);
```

+ RETURN VALUES

     Upon successful completion, a value of 0 is returned.  Otherwise, a value of -1 is returned and the global integer variable errno
     is set to indicate the error.

当connect到达时候，accept中的fd变为可写，当三次握手完成，accept从监听队列中取出这个完成的fd，如果握手失败，协议会把这个fd从监听队列中删除，accept会继续阻塞。所以IO服用的服务端监听fd一般设置为非阻塞。

无阻塞connect 可以立刻返回，根据返回值和 errno 处理三种情况：

1. 如果返回 0，表示 connect 成功。
2. 如果返回值小于 0， errno 为 EINPROGRESS,  表示连接建立已经启动但是尚未完成。这是期望的结果，不是真正的错误。
3. 如果返回值小于0，errno 不是 EINPROGRESS，则连接出错了。

connect会阻塞，怎么解决?
最通常的方法最有效的是加定时器,同时采用非阻塞模式,之后用select检测状态,

无阻塞connect demo
```c
if (connect(sock, (struct sockaddr *)(&rsock), sizeof(rsock)) == -1) {
   //表示此时tcp三次握手仍旧进行，如果errno不是EINPROGRESS，则说明连接错误，程序结束
   if (errno != EINPROGRESS) {
     return 0;
   } else {
     tm.tv_sec = 5;
     tm.tv_usec = 0;
     FD_ZERO(&wset);
     FD_SET(sock, &wset);
     //监听写集合
     if (select(sock + 1, NULL, &wset, NULL, &tm) > 0) {
       //函数返回值来判断是否发生错误 error返回0则表示连接成功！
       getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&slen);
       if (error == 0) {
         socket_set_block(sock);
         return sock;
       } else {
         return 0;
       }
     } else {
       return 0;
     }
   }
 } else {
   socket_set_block(sock);
   return sock;
}
```

#### 收发函数

```c
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
              const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
```

## 网络编程

### 设置socket为非阻塞non-blocking
使用socket()创建的socket(file descriptor)，默认是阻塞的(blocking)；使用函数fcntl()(file control)可设置创建的socket为非阻塞的non-blocking。
```c
#include <unistd.h>
#include <fcntl.h>

sock = socket(PF_INET, SOCK_STREAM, 0);

int flags = fcntl(sock, F_GETFL, 0);
fcntl(sock, F_SETFL, flags | O_NONBLOCK); 
```

### select

```c
int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);
``` 

1. `int maxfdp` 是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！
在Windows中这个参数的值无所谓，可以设置不正确。   
　　
2. `fd_set *readfds`
是指向fd_set结构的指针，这个集合中应该包括文件描述符，
我们是要监视这些文件描述符的读变化的，即我们关心是否可以从这些文件中读取数据了，
如果这个集合中有一个文件可读，select就会返回一个大于0的值，表示有文件可读，
如果没有可读的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，
若发生错误返回负值。可以传入NULL值，表示不关心任何文件的读变化。 
　　
3. `fd_set *writefds`
是指向fd_set结构的指针。 
　　
4. `fd_set *errorfds`
同上面两个参数的意图，用来监视文件错误异常。 
　　
5. `struct timeval* timeout`
是select的超时时间，这个参数至关重要，它可以使select处于三种状态：
第一，若将NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止；
第二，若将时间值设为0秒0毫秒，就变成一个纯粹的非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行，文件无变化返回0，有变化返回一个正值；
第三，timeout的值大于0，这就是等待的超时时间，即 select在timeout时间内阻塞，超时时间之内有事件到来就返回了，否则在超时后不管怎样一定返回，返回值同上述。

返回值： 

负值：select错误; 正值：某些文件可读写或出错; 0：等待超时，没有可读写或错误的文件

struct fd_set 是fd集合由宏来操作，

1. 比如清空集合 `FD_ZERO(fd_set *)`，
2. 将一个给定的文件描述符加入集合之中`FD_SET(int ,fd_set *)`，
3. 将一个给定的文件描述符从集合中删除`FD_CLR(int ,fd_set*)`，
4. 检查集合中指定的文件描述符是否可以读写`FD_ISSET(int ,fd_set* )`。


