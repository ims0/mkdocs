# IPC

## UNIX 版本

多数UNIX 系统或者源自 Berkeley，或者来自 System V
大多数厂家一开始采用 Posix 标准，来减少这些差别。

[systemV与posix对比](https://blog.csdn.net/firstlai/article/details/50705042)

###  posix IPC 函数

|type | 消息队列 | 信号量 | 共享内存|
|-- | -- | -- | --- |
|头文件|mqueue.h | semaphore.h | sys/mman.h |
|删建函数 | mq_open,mq_close, mq_unlink | sem_open, sem_close, sem_unlink; sem_init, sem_destory| shm_open, shm_unlink|
|控制IPC函数| mq_getattr, mq_setattr | | ftruncate, fstat|
| 操作函数 | mq_send, mq_receive, mq_notify | sem_init, sem_trywait, sem_post, sem_getvalue| mmap, munmap|

----
## 多线程涉及的4个主要领域

1. 消息传递（管道，FIFO，消息队列）
2. 同步（互斥锁，条件变量，读写锁，信号量）
3. 共享内存区
4. 过程调用（Solaris 门， Sun RPC）

## 消息队列

消息队列具有随内核的持续性，一个进程发送完成退出，另一个进程再去读取。
### posix与systemV消息队列的差异

1. posix消息队列总是返回最高优先级的最早消息，SystemV消息队列可以返回任意指定优先级的消息
2. 当往一个空队列放置消息的时候，Posix消息队列允许产生一个信号或启动一个线程，SystemV没有类似的机制

队列中每个消息有如下的属性(优先级，长度，数据)

1. 一个无符号整型优先级(posix), 或一个长整型类型(systemV)
2. 消息的数据部分可以为0
3. 数据本身(如果长度大于0，则有数据)

### 消息队列问题

消息队列的作用？

1. 程序解耦
2. 数据冗余，例如rabbitmq的ack机制，消息确认机制
3. 削峰能力
4. 可恢复性，就算系统中部分组件挂掉，消息在队列也不丢失，待组件恢复后继续处理消息。
5. 异步通信，如发红包，短信等流程丢入队列，可以优先级很低的去处理。

## 信号量 semaphore

### 三种不同的信号量

1. Posix有名信号量:使用Posix IPC名字标识，可用于进程或线程间的同步。
2. Posix基于内存的信号量:存放在共享内存区，可用于进程或线程间的同步。
3. SystemV 信号量：在内核中维护，可用于进程或线程间的同步。

### Posix 信号量是否共享
+ Posix有名信号量使用name参数标识，它通常指代文件系统中的某个文件。
+ 无名信号量就是基于内存的信号量，由应用程序分配内存空间，使用`sem_init(sem_t*sem, int shared, unsigned value)`初始化，
    其中sem参数指向程序分配的sem_t变量，如果shared为0，则信号量在进程内线程间共享，否则在进程间共享。当shared非零时，该信号量必须放在共享内存，
    让所有进程都能访问到它，vlaue为初始值。


### 信号量，互斥锁，条件变量差异

1. 互斥锁必须总是由给它上锁的线程解锁，信号量的挂出却不必由执行过它的等待操作的同一线程执行。
2. 互斥锁要么被锁住，要么被解开 （二值状态，类似二值信号量）
3. 既然信号量有个与之关联的状态（它的计数值），那么信号量的挂出操作总是被记住。然而当一个条件变量发送信号时，所有没有线程等待在该条件变量上，那么该信号将消失。


## 共享内存

由于数据传递不涉及内核，所以共享内存是IPC中速度最快的，存取数据需要各种形式的同步:互斥锁，条件变量，读写锁，记录锁，信号量。

### mmap,munmap,msync

mmap函数把一个文件或一个Posix共享内存区对象映射到调用进程的地址空间，使用函数三个目的:

1. 使用普通文件以提供内存映射I/O
2. 使用特殊文件以提供内存匿名映射
3. 使用shm_open以提供无亲缘关系进程间的Posix共享内存区

`void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)`

Posix共享内存构筑在mmap函数之上，首先指定一个Posix IPC名字来调用shm_open(),取得描述符后，使用mmap把它映射到内存，类似于内存映射文件。
如果用内存映射文件的形式来实现Posix共享内存，只需要把shm_open()调用open(),shm_unlink调用unlink实现。

### SystemV共享内存

类似Posix共享内存，先调shmget,再调shmat

#### shmget

创建一个新的共享内存区，或者访问一个已经存在的共享内存区

#### shmat

把共享内存附接到调用进程的地址空间







## 管道

管道有：匿名管道，有名管道（FIFO）
两者默认都是半双工， 一端用来写一端用来读。
```
int pipe(int fd[2])
int mkfifo(const char *pathname, mode_t mode)
```
+ 全双工使用socket函数

`int socketpair(int domain, int type, int protocol, int sv[2]);`


