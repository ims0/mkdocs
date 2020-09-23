[toc]

#

## 进程

内核把进程的列表放在叫做任务队列的双向循环链表中，每个节点是进程描述符定义在<linux/sched.h>,描述了一个正在执行的程序：打开的文件，地址空间，挂起信号，进程的状态···。

### 进程的5种状态

+ TASK_RUNNING (运行)---进程是可执行的，它要么在执行，要么在运行队列中等待执行，
+ TASK_INTERRUPTIBLE(可中断)---正在睡眠或说是阻塞，一旦条件达成，就投入运行。收到信号可能提前被唤醒。
+ TASK_UNINTERRUPTIBLE(不可中断)--- 收到信号也不会准备投入运行，这个状态出现在进程等待时不能受到干扰，或者等待条件很快就会出现。处于这个状态的进程对信号不做响应。
+ TASK_TRACED ---被其它程序跟踪的进程，如ptrace对调试程序跟踪
+ TASK_STOPPED(停止) ---停止执行，没有运行也不能运行，这个状态发生在收到:SIGSTOP,SIGTSTP,SIGTTIN,SIGTTOU 等信号的时候，在调试期间收到任何信号都会让程序进入这个状态。

### 僵尸进程

目的是:维护子进程信息，供父进程后续获取，信息包括（PID，终止状态，资源利用），有些UNIX的ps命令的COMMAND栏：以`<defunct>`表示僵尸进程，
有些linux系统state栏，以`Z`表示僵尸进程。

## linux命令
### ps

To see every process on the system using standard syntax:
```
  ps -e
  ps -ef
  ps -eF
  ps -ely
```
To see every process on the system using BSD syntax:

```
  ps ax
  ps axu
```
To print a process tree:
```
  ps -ejH
  ps axjf
```
To get info about threads:
```
  ps -eLf
  ps axms
```
To get security info:
```
  ps -eo euser,ruser,suser,fuser,f,comm,label
  ps axZ
  ps -eM
```
#### pstree
以进程树的形式查看进程

#### PROCESS STATE CODES         top
       Here are the different values that the s, stat and state output
       specifiers (header "STAT" or "S") will display to describe the state
       of a process:

               D    uninterruptible sleep (usually IO)
               I    Idle kernel thread
               R    running or runnable (on run queue)
               S    interruptible sleep (waiting for an event to complete)
               T    stopped by job control signal
               t    stopped by debugger during the tracing
               W    paging (not valid since the 2.6.xx kernel)
               X    dead (should never be seen)
               Z    defunct ("zombie") process, terminated but not reaped by
                    its parent

       For BSD formats and when the stat keyword is used, additional
       characters may be displayed:

               <    high-priority (not nice to other users)
               N    low-priority (nice to other users)
               L    has pages locked into memory (for real-time and custom
                    IO)
               s    is a session leader
               l    is multi-threaded (using CLONE_THREAD, like NPTL
                    pthreads do)
               +    is in the foreground process group


#### top

#### kill

### objdump


#### objdump反汇编常用参数
objdump -d <file(s)>: 将代码段反汇编；
objdump -S <file(s)>: 将代码段反汇编的同时，将反汇编代码与源代码交替显示，编译时需要使用-g参数，即需要调试信息；
objdump -C <file(s)>: 将C++符号名逆向解析
objdump -l <file(s)>: 反汇编代码中插入文件名和行号
objdump -j section <file(s)>: 仅反汇编指定的section

#### 显示main.c的汇编代码
gcc -S -o main.s main.c


#### 目标文件反汇编
gcc -c -o main.o main.c
objdump -s -d main.o > main.o.txt

### nm命令

nm是names的缩写， nm命令主要是用来列出某些文件中的符号（说白了就是一些函数和全局变量等）

语法格式
nm [参数]
常用选项：

-A    每个符号前显示文件名
-D    显示动态符号
-g    仅显示外部符号
-r    反序显示符号表

### netstat

### lsof

### nc

https://www.cnblogs.com/bakari/p/10898604.html

### tcpdump

https://www.cnblogs.com/bakari/p/10748721.html

### df
### fdisk
### ifconfig

### uname

常用 `uname -a`
```
 -a, --all
        print all information, in the following order, except omit -p and -i if unknown:

 -s, --kernel-name
        print the kernel name

 -n, --nodename
        print the network node hostname

 -r, --kernel-release
        print the kernel release

 -v, --kernel-version
        print the kernel version

 -m, --machine
        print the machine hardware name

 -p, --processor
        print the processor type (non-portable)

 -i, --hardware-platform
        print the hardware platform (non-portable)

 -o, --operating-system
        print the operating system

```

### ipcs

```
 Resource options
     -m, --shmems
            Write information about active shared memory segments.

     -q, --queues
            Write information about active message queues.

     -s, --semaphores
            Write information about active semaphore sets.

     -a, --all
            Write information about all three resources (default).

```

#### ipcrm

```
  -a, --all [shm] [msg] [sem]
         Remove  all resources.  When an option argument is provided, the removal is performed only for the specified resource types.  Warning!
         Do not use -a if you are unsure how the software using the resources might react to missing objects.  Some programs create  these  re‐
         sources at startup and may not have any code to deal with an unexpected disappearance.

  -M, --shmem-key shmkey
         Remove the shared memory segment created with shmkey after the last detach is performed.

  -m, --shmem-id shmid
         Remove the shared memory segment identified by shmid after the last detach is performed.

  -Q, --queue-key msgkey
         Remove the message queue created with msgkey.

  -q, --queue-id msgid
         Remove the message queue identified by msgid.

  -S, --semaphore-key semkey
         Remove the semaphore created with semkey.

  -s, --semaphore-id semid
         Remove the semaphore identified by semid.
```

## linux大小端问题

1. Little-Endian就是低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。
2. Big-Endian就是高位字节排放在内存的低地址端，低位字节排放在内存的高地址端。

举一个例子，比如数字高位在左边 0x12 34 56 78在内存中的表示形式为：

1. 大端模式：高位也在左边，阅读顺序一致，高位低地址
```
低地址 -----------------> 高地址
0x12  |  0x34  |  0x56  |  0x78
```

2. 小端模式：
```
低地址 ------------------> 高地址
0x78  |  0x56  |  0x34  |  0x12
```

### 大端小端各自优势：
1. 小端模式 ：强制转换数据不需要调整字节内容，1、2、4字节的存储方式一样。
2. 大端模式 ：符号位的判定固定为第一个字节，容易判断正负。


一般操作系统都是小端，而通讯协议是大端的。

常见CPU的字节序
Big Endian : PowerPC、IBM、Sun
Little Endian : x86、DEC
ARM既可以工作在大端模式，也可以工作在小端模式。
```
BOOL GetEndian()
{
    int a = 0x1234;
    //通过将int强制类型转换成char单字节，通过判断起始存储位置。即等于 取b等于a的低地址部分
    char b =  *(char *)&a;
    if( b == 0x12)
    {
        return BigEndian;
    }
    return SmallEndian;
}
```
联合体union的存放顺序是所有成员都从低地址开始存放，利用该特性可以轻松地获得了CPU对内存采用Little-endian还是Big-endian模式读写：
```
BOOL IsBigEndian()
{
    union NUM
    {
        int a;
        char b;
    }num;
    num. a = 0x1234;
    if( num. b == 0x12 )
    {
        return TRUE;
    }
    return FALSE;
}
```


## linux 网络编程


### [络编程之listen函数](https://blog.csdn.net/godop/article/details/79894079)
```
SYNOPSIS
       #include <sys/types.h>          /* See NOTES */
       #include <sys/socket.h>

       int listen(int sockfd, int backlog);
DESCRIPTION
       listen()  marks  the  socket  referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection re‐
       quests using accept(2).

       The sockfd argument is a file descriptor that refers to a socket of type SOCK_STREAM or SOCK_SEQPACKET.

       The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.  If a connection  request  arrives
       when  the  queue  is full, the client may receive an error with an indication of ECONNREFUSED or, if the underlying protocol supports retransmis‐
       sion, the request may be ignored so that a later reattempt at connection succeeds.

RETURN VALUE
       On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
```

![avatar](tcp_ip_pic/listen_func.png)
![avatar](tcp_ip_pic/listen_queue.png)


### recv/recvfrom/recvmsg

```
SYNOPSIS
       #include <sys/types.h>
       #include <sys/socket.h>

       ssize_t recv(int sockfd, void *buf, size_t len, int flags);

       ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);

       ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
```

## 内存泄漏排查

### 一、重载new/delete操作符
重载new/delete操作符，用list或者map记录对内存的使用情况。new一次，保存一个节点，delete一次，就删除节点。
最后检测容器里是否还有节点，如果有节点就是有泄漏。也可以记录下哪一行代码分配的内存被泄漏。
类似的方法：在每次调用new时加个打印，每次调用delete时也加个打印。
### 二、使用mtrace/muntrace
linux 提供mtrace/muntrace来检测程序是否有内存泄露。一般来说要检测哪一段代码是否有内存泄露，就可以用这一对函数包起来。

每一对malloc-free的执行，若每一个malloc都有相应的free，则代表没有内存泄露，对于任何非malloc/free情況下所发生的内存泄露问题，mtrace并不能找出来。

在使用mtrace之前，先要设置一个环境变量“MALLOC_TRACE”来指定mtrace检测结果的生成文件名。通过此文件就可以看出代码是否有内存泄露。
```

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
```
运行test_mtrace，就会生成指定文件mtrace_output.此文件是一个txt类型。

用mtrace命令分析mtrace的log文件：`mtrace [binary] mtrace_output`。
就会将output_file_name的內容转化成能被理解的语句。

### 三、查看进程maps表

在实际调试过程中，怀疑某处发生了内存泄漏，可以查看该进程的maps表，看进程的堆或mmap段的虚拟地址空间是否持续增加。如果是，说明可能发生了内存泄漏。如果mmap段虚拟地址空间持续增加，还可以看到各个段的虚拟地址空间的大小，从而可以确定是申请了多大的内存。

### 四、valgrind工具

### [基于链表的C语言堆内存检测](https://blog.csdn.net/hanyin7/article/details/38377743)
本文基于链表实现C语言堆内存的检测机制，可检测内存泄露、越界和重复释放等操作问题。
本文仅提供即视代码层面的检测机制，不考虑编译链接级的注入或钩子。此外，该机制暂未考虑并发保护。


