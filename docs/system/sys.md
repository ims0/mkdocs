
## 虚拟内存：
![avitor](sys_pic/virtual_mem.png)
术语：
### DRAM
DRAM缓存来表示虚拟内存系统中的缓存,它在主存中缓存虚拟页。


### MMU(Memory Management Unit)
CPU芯片上的内存管理单元利用内存中的查询表来翻译虚拟地址，表内容由操作系统管理。

### 快表TBL（Translation Lookaside Buffer)
是MMU中关于PTE的小缓存示虚拟内存系统的缓存，它在主存中缓存虚拟页。
### 页表
页表：存放在内存中的页表条目数组，将虚拟页映射到物理页；
虚拟地址空间中的每个页在页表中一个偏移量处都有一个PTE（Page Table Entry），   

假定PTE由 **有效位** 和 **地址字段** 组成。

|有效位 | 地址字段| 含义| 
|--|--|--
1 | ptr=0x111 | ptr表示DRAM中对应物理页的首地址
0 | ptr=0x111 | ptr表示该页在磁盘上的首地址
0 | ptr=NULL | ptr表示该页未分配



### 页命中

CPU 访问虚拟地址时候，地址翻译硬件将虚拟地址作为索引定位到PTE，当时PTE设置了有效位时候，
说明虚拟页缓存在内存中了，PTE中的地址字段就是内存中该页的首地址。

### 缺页

DRAM不命中成为缺页(page fault)。

### 多级页表

以32位系统为例，页大小为4kb，PTE为4字节。需要4M的页表留在内存中。其中只使用来虚拟地址空间的一小部分。

####  多级页表两方面减少内存使用

1. 如果一个一级页表的PTE是空的，那么二级页表就不会存在
2. 只有一级页表才需要驻留在内存中，只有经常使用的二级页表才会缓存在主存中。

## smp
The mfence instruction is specific to x86/64. If you want to make the code more portable, 
you could wrap this intrinsic in a preprocessor macro. The Linux kernel has wrapped it in a macro named `smp_mb`,
along with related macros such as `smp_rmb` and `smp_wmb`, and provided alternate implementations on different architectures.
For example, on PowerPC, smp_mb is implemented as sync.

https://preshing.com/20120515/memory-reordering-caught-in-the-act/
```c
void *thread1Func(void *param) {
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
    printf("set thread affinity failed! \n");
  }
  srand(time(NULL)); // Initialize random number generator
  for (;;)           // Loop indefinitely
  {
    sem_wait(&beginSema1); // Wait for signal from main thread
    while (rand() % 8 != 0) {
    } // Add a short, random delay

    // ----- THE TRANSACTION! -----
    X = 1;
    // asm volatile("" ::: "memory");        // Prevent compiler reordering
    // asm volatile("mfence" ::: "memory");  // Prevent memory reordering
    r1 = Y;

    sem_post(&endSema); // Notify transaction complete
  }
  return NULL; // Never returns
};
```


## 死锁
### 死锁必要条件及避免算法、

1. 资源不能共享，只能由一个进程使用。
2. 不可剥夺（Nopre-emption）：已经分配的资源不能从相应的进程中被强制地剥夺。
3. 请求与保持（Hold andwait）：已经得到资源的进程可以再次申请新的资源。
4. 循环等待：系统中若干进程组成环路，该环路中每个进程都在等待相邻进程正占用的资源

### 处理死锁的策略：
通过破除死锁四个必要条件之一，来防止死锁产生。

1. 保证相同的加锁顺序避免死锁。（避免环路）
2. 检测死锁并且恢复。(破坏请求与保持)
    1. 请求资源时候检测，获取不到资源就释放已经获取的资源
    2. 定时检测是否发生死锁
### c++11 处理死锁
1. [std::lock](https://zh.cppreference.com/w/cpp/thread/lock)
锁定给定的可锁定 (Lockable) 对象 lock1 、 lock2 、 ... 、 lockn ，用免死锁算法避免死锁。
以对 lock 、 try_lock 和 unlock 的未指定系列调用锁定对象。若调用 lock 或 unlock 导致异常，则在重抛前对任何已锁的对象调用 unlock 。

```cpp
// std::lock example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock

std::mutex foo,bar;

void task_a () {
  // foo.lock(); bar.lock(); // replaced by:
  std::lock (foo,bar);
  std::cout << "task a\n";
  foo.unlock();
  bar.unlock();
}

void task_b () {
  // bar.lock(); foo.lock(); // replaced by:
  std::lock (bar,foo);
  std::cout << "task b\n";
  bar.unlock();
  foo.unlock();
}

int main ()
{
  std::thread th1 (task_a);
  std::thread th2 (task_b);

  th1.join();
  th2.join();

  return 0;
}
```
## 无锁编程
https://juejin.cn/post/6844903486593695751    
https://docs.microsoft.com/zh-cn/windows/win32/dxtecharts/lockless-programming?redirectedfrom=MSDN

Lockless 编程
顾名思义，Lockless 编程是一系列方法，用于在不使用锁的情况下安全地操作共享数据。 有可用于传递消息、共享数据的列表和队列以及其他任务的 lockless 算法。
执行 lockless 编程时，必须处理两个难题：非原子操作和重新排序。

### rcu
userspace RCU (read-copy-update)    
http://liburcu.org/

### 原子变量
atomic<int> i = 0;

### [利用CAS实现线程安全的无锁队列](https://blog.csdn.net/qq_40843865/article/details/90734118)
在开始说无锁队列之前，我们需要知道一个很重要的技术就是CAS操作——Compare And Set 或是 Compare And Swap，比较并交换是原子操作的一种，可用于在多线程编程中实现不被打断的数据交换操作，从而避免多线程同时改写某一数据时由于执行顺序不确定性以及中断的不可预知性产生的数据不一致问题。该操作通过将内存中的值与指定值oldval进行比较，当内存中的值与oldval一样时将内存中的值替换为新值newval。 现在几乎所有的CPU指令都支持CAS的原子操作，X86下对应的是 `CMPXCHG` 汇编指令。有了这个原子操作，我们就可以用其来实现各种无锁（lock free）的数据结构。

### C++ 的新并发哈希映射
https://preshing.com/20160201/new-concurrent-hash-maps-for-cpp/    
https://github.com/preshing/junction

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

### 辨别系统是16位or32位

法一：int k=~0;  
法二：sizeof(void*) ;

## 信号

    信号机制是进程之间相互传递消息的一种方法，信号全称为软中断信号，也有人称作软中断。
    进程之间可以互相通过系统调用kill发送软中断信号。
    SIGHUP 1 A 终端挂起或者控制进程终止
    SIGINT 2 A 键盘中断（如break键被按下）
    SIGQUIT 3 C 键盘的退出键被按下
    SIGILL 4 C 非法指令
    SIGABRT 6 C 由abort(3)发出的退出指令
    SIGFPE 8 C 浮点异常
    SIGKILL 9 AEF Kill信号
    SIGSEGV 11 C 无效的内存引用
    SIGPIPE 13 A 管道破裂: 写一个没有读端口的管道

信号机制是异步的；当一个进程接收到一个信号时，它会立刻处理这个信号，而不会等待当前函数甚至当前一行代码结束运行。信号有几十种，分别代表着不同的意义。信号之间依靠它们的值来区分，但是通常在程序中使用信号的名字来表示一个信号。在Linux系统中，这些信号和以它们的名称命名的常量均定义在/usr/include/bits/signum. h文件中。（通常程序中不需要直接包含这个头文件，而应该包含<signal. h>。）

信号事件的发生有两个来源：硬件来源(比如我们按下了键盘或者其它硬件故障)；软件来源，最常用发送信号的系统函数是kill, raise, alarm和setitimer以及sigqueue函数，软件来源还包括一些非法运算等操作。

发送信号的主要函数有：kill()、raise()、 sigqueue()、alarm()、setitimer()以及abort()。



进程可以通过三种方式来响应一个信号：（1）忽略信号，即对信号不做任何处理，其中，有两个信号不能忽略：SIGKILL及SIGSTOP；（2）捕捉信号。定义信号处理函数，当信号发生时，执行相应的处理函数；（3）执行缺省操作，


## 实现守护进程的步骤

守护进程最重要的特性是后台运行。

1.  在后台运行。
    为避免挂起控制终端将Daemon放入后台执行。方法是在进程中调用fork使父进程终止，让Daemon在子进程中后台执行。

    if(pid=fork())
    exit(0); //是父进程，结束父进程，子进程继续

2.  脱离控制终端，登录会话和进程组

    有必要先介绍一下Linux中的进程与控制终端，登录会话和进程组之间的关系：进程属于一个进程组，进程组号（GID）就是进程组长的进程号（PID）。登录会话可以包含多个进程组。这些进程组共享一个控制终端。这个控制终端通常是创建进程的登录终端。控制终端，登录会话和进程组通常是从父进程继承下来的。我们的目的就是要摆脱它们，使之不受它们的影响。方法是在第1点的基础上，调用setsid()使进程成为会话组长：

    setsid();

    说明：当进程是会话组长时setsid()调用失败。但第一点已经保证进程不是会话组长。setsid()调用成功后，进程成为新的会话组长和新的进程组长，并与原来的登录会话和进程组脱离。由于会话过程对控制终端的独占性，进程同时与控制终端脱离。

3.  禁止进程重新打开控制终端

    现在，进程已经成为无终端的会话组长。但它可以重新申请打开一个控制终端。可以通过使进程不再成为会话组长来禁止进程重新打开控制终端：

    if(pid=fork()) exit(0); //结束第一子进程，第二子进程继续（第二子进程不再是会话组长）

4.  关闭打开的文件描述符

    进程从创建它的父进程那里继承了打开的文件描述符。如不关闭，将会浪费系统资源，造成进程所在的文件系统无法卸下以及引起无法预料的错误。按如下方法关闭它们：

    for(i=0;i 关闭打开的文件描述符close(i);>

5.  改变当前工作目录

    进程活动时，其工作目录所在的文件系统不能卸下。一般需要将工作目录改变到根目录。对于需要转储核心，写运行日志的进程将工作目录改变到特定目录如 /tmpchdir("/")

6.  重设文件创建掩模

    进程从创建它的父进程那里继承了文件创建掩模。它可能修改守护进程所创建的文件的存取位。为防止这一点，将文件创建掩模清除：umask(0);

7.  处理SIGCHLD信号

    处理SIGCHLD信号并不是必须的。但对于某些进程，特别是服务器进程往往在请求到来时生成子进程处理请求。如果父进程不等待子进程结束，子进程将成为僵尸进程（zombie）从而占用系统资源。如果父进程等待子进程结束，将增加父进程的负担，影响服务器进程的并发性能。在Linux下可以简单地将 SIGCHLD信号的操作设为SIG_IGN。

    signal(SIGCHLD,SIG_IGN);

    这样，内核在子进程结束时不会产生僵尸进程。这一点与BSD4不同，BSD4下必须显式等待子进程结束才能释放僵尸进程。

### 标准库函数和系统调用的区别

1. 系统调用

    系统调用提供的函数如open, close, read, write, ioctl等，需包含头文件unistd. h。以write为例：其函数原型为 size_t write(int fd, const void *buf, size_t nbytes)，其操作对象为文件描述符或文件句柄fd(file descriptor)，要想写一个文件，必须先以可写权限用open系统调用打开一个文件，获得所打开文件的fd，例如fd=open(/"/dev/video/", O_RDWR)。fd是一个整型值，每新打开一个文件，所获得的fd为当前最大fd加1。Linux系统默认分配了3个文件描述符值：0－standard input，1－standard output，2－standard error。

    系统调用通常用于底层文件访问（low-level file access），例如在驱动程序中对设备文件的直接访问。
    系统调用是操作系统相关的，因此一般没有跨操作系统的可移植性。

    系统调用发生在内核空间，因此如果在用户空间的一般应用程序中使用系统调用来进行文件操作，会有用户空间到内核空间切换的开销。事实上，即使在用户空间使用库函数来对文件进行操作，因为文件总是存在于存储介质上，因此不管是读写操作，都是对硬件（存储器）的操作，都必然会引起系统调用。也就是说，库函数对文件的操作实际上是通过系统调用来实现的。例如C库函数fwrite()就是通过write()系统调用来实现的。

    这样的话，使用库函数也有系统调用的开销，为什么不直接使用系统调用呢？这是因为，读写文件通常是大量的数据（这种大量是相对于底层驱动的系统调用所实现的数据操作单位而言），这时，使用库函数就可以大大减少系统调用的次数。这一结果又缘于缓冲区技术。在用户空间和内核空间，对文件操作都使用了缓冲区，例如用fwrite写文件，都是先将内容写到用户空间缓冲区，当用户空间缓冲区满或者写操作结束时，才将用户缓冲区的内容写到内核缓冲区，同样的道理，当内核缓冲区满或写结束时才将内核缓冲区内容写到文件对应的硬件媒介。

2. 库函数调用(可移植性好,驱动不可用)

    标准C库函数提供的文件操作函数如fopen, fread, fwrite, fclose,fflush, fseek等，需包含头文件stdio. h。以fwrite为例，其函数原型为size_t fwrite(const void *buffer,size_t size, size_t item_num, FILE *pf)，其操作对象为文件指针FILE *pf，要想写一个文件，必须先以可写权限用fopen函数打开一个文件，获得所打开文件的FILE结构指针pf，例如pf=fopen(/"~/proj/filename/",/"w/")。实际上，由于库函数对文件的操作最终是通过系统调用实现的，因此，每打开一个文件所获得的FILE结构指针都有一个内核空间的文件描述符fd与之对应。同样有相应的预定义的FILE指针：stdin－standard input，stdout－standard output，stderr－standard error。

    库函数调用通常用于应用程序中对一般文件的访问。
    库函数调用是系统无关的，因此可移植性好。
    由于库函数调用是基于C库的，因此也就不可能用于内核空间的驱动程序中对设备的操作

## 常见问题

### PCB
32位系统一个进程最多有多少堆内存?

+ 32位系统进程地址空间是4G, 用户可访问的空间是3G 左右。
+ 一个线程的默认栈大小是`ulimit -s` 8M, 一个进程最多起300个线程。

### 惊群现象
对于操作系统来说，多个进程/线程在等待同一资源是,其结果就是每当资源可用，所有的进程/线程都来竞争资源，造成的后果：

*  1. 系统对用户进程/线程频繁的做无效的调度、上下文切换，系统系能大打折扣。
*  2. 为了确保只有一个线程得到资源，用户必须对资源操作进行加锁保护，进一步加大了系统开销。

#### 什么是惊群
最常见的例子就是对于socket描述符的accept操作，当多个用户进程/线程监听在同一个端口上时，由于实际只可能accept一次，因此就会产生惊群现象，当然前面已经说过了，这个问题是一个古老的问题， **新的操作系统内核已经解决了这一问题**。

#### 解决方式:独占等待

当一个进程调用 wake_up 在等待队列上，所有的在这个队列上等待的进程被置为可运行的。这在许多情况下是正确的做法。但有时，可能只有一个被唤醒的进程将成功获得需要的资源，而其余的将再次休眠。这时如果等待队列中的进程数目大，这可能严重降低系统性能。为此，内核开发者增加了一个“独占等待”选项。它与一个正常的睡眠有 2 个重要的不同:

1. 当等待队列入口设置了 `WQ_FLAG_EXCLUSEVE` 标志，它被添加到等待队列的尾部；否则，添加到头部。

2. 当 wake_up 被在一个等待队列上调用, 它在唤醒第一个有 WQ_FLAG_EXCLUSIVE 标志的进程后停止唤醒.但内核仍然每次唤醒所有的非独占等待。

采用独占等待要满足 2 个条件:

1. 希望对资源进行有效竞争；
2. 当资源可用时，唤醒一个进程就足够来完全消耗资源。

使一个进程进入独占等待，可调用：
```c
void prepare_to_wait_exclusive(wait_queue_head_t *queue, wait_queue_t *wait, int state);

void add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait)
{
    unsigned long flags;

    wait->flags &= ~WQ_FLAG_EXCLUSIVE;
    spin_lock_irqsave(&q->lock, flags);
    __add_wait_queue(q, wait);
    spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL(add_wait_queue);

void add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait)
{
    unsigned long flags;

    wait->flags |= WQ_FLAG_EXCLUSIVE;
    spin_lock_irqsave(&q->lock, flags);
    __add_wait_queue_tail(q, wait);
    spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL(add_wait_queue_exclusive);
```
注意：无法使用 wait_event 和它的变体来进行独占等待.

也就是说，对于互斥等待的行为，比如如对一个listen后的socket描述符，多线程阻塞accept时，系统内核只会唤醒所有正在等待此时间的队列的第一个，队列中的其他人则继续等待下一次事件的发生，这样就避免的多个线程同时监听同一个socket描述符时的惊群问题。

#### 惊群问题出现场景
Linux2.6内核版本之前系统API中的accept调用

在Linux2.6内核版本之前，当多个线程中的accept函数同时监听同一个listenfd的时候，如果此listenfd变成可读，则系统会唤醒所有使用accept函数等待listenfd的所有线程（或进程），但是最终只有一个线程可以accept调用返回成功，其他线程的accept函数调用返回EAGAIN错误，线程回到等待状态，这就是accept函数产生的惊群问题。但是在Linux2.6版本之后，内核解决了accept函数的惊群问题，当内核收到一个连接之后，只会唤醒等待队列上的第一个线程（或进程），从而避免了惊群问题。

<font color=#0022ff>epoll函数中的惊群问题</font>

如果我们使用多线程epoll对同一个fd进行监控的时候，当fd事件到来时，内核会把所有epoll线程唤醒，因此产生惊群问题。为何内核不能像解决accept问题那样解决epoll的惊群问题呢？内核可以解决accept调用中的惊群问题，是因为内核清楚的知道accept调用只可能一个线程调用成功，其他线程必然失败。而对于epoll调用而言，内核不清楚到底有几个线程需要对该事件进行处理，所以只能将所有线程全部唤醒。

<font color=#0022ff>epoll惊群解决办法</font>

1. 对于epll函数调用的惊群问题解决办法可以参考Nginx的解决办法，多个进程将listenfd加入到epoll之前，首先尝试获取一个全局的accept_mutex互斥锁，只有获得该锁的进程才可以把listenfd加入到epoll中，当网络连接事件到来时，只有epoll中含有listenfd的线程才会被唤醒并处理网络连接事件。从而解决了epoll调用中的惊群问题。(epoll也可以避免惊群，4.5内核加入了EPOLLEXCLUSIVE选项。)

    a. nginx-1.20.1/src/event/ngx_event.c
```c
462:extern ngx_atomic_t          *ngx_accept_mutex_ptr;
463:extern ngx_shmtx_t            ngx_accept_mutex;
464:extern ngx_uint_t             ngx_use_accept_mutex;
466:extern ngx_uint_t             ngx_accept_mutex_held;
467:extern ngx_msec_t             ngx_accept_mutex_delay;
```

    b. v5.14/include/uapi/linux/eventpoll.h
```c 
/* Set exclusive wakeup mode for the target file descriptor */
#define EPOLLEXCLUSIVE    ((__force __poll_t)(1U << 28))

/*
 * epoll adds to the wakeup queue at EPOLL_CTL_ADD time only,
 * so EPOLLEXCLUSIVE is not allowed for a EPOLL_CTL_MOD operation.
 * Also, we do not currently supported nested exclusive wakeups.
 */
if (ep_op_has_event(op) && (epds->events & EPOLLEXCLUSIVE)) {
    if (op == EPOLL_CTL_MOD)
        goto error_tgt_fput;
    if (op == EPOLL_CTL_ADD && (is_file_epoll(tf.file) ||
            (epds->events & ~EPOLLEXCLUSIVE_OK_BITS)))
        goto error_tgt_fput;
}
```

<font color=#0022ff>线程池中的惊群问题</font>

在实际应用程序开发中，为了避免线程的频繁创建销毁，我们一般建立线程池去并发处理，而线程池最经典的模型就是生产者-消费者模型，包含一个任务队列，当队列不为空的时候，线程池中的线程从任务队列中取出任务进行处理。一般使用条件变量进行处理，当我们往任务队列中放入任务时，需要唤醒等待的线程来处理任务，如果我们使用C++标准库中的函数notify_all()来唤醒线程，则会将所有的线程都唤醒，然后最终只有一个线程可以获得任务的处理权，其他线程在此陷入睡眠，因此产生惊群问题。

<font color=#0022ff>线程池惊群解决</font>
对于线程池中的惊群问题，我们需要分情况看待，有时候业务需求就是需要唤醒所有线程，那么这时候使用notify_all()唤醒所有线程就不能称为”惊群问题“，因为CPU并没有无谓消耗。而对于只需要唤醒一个线程的情况，我们需要使用notify_one()函数代替notify_all()只唤醒一个线程，从而避免惊群问题。


#### 抢锁不也是一个惊群

抢锁是由操作系统在阻塞线程里挑一个唤醒。
