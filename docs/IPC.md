# IPC

## UNIX 版本

多数UNIX 系统或者源自 Berkeley，或者来自 System V
大多数厂家一开始采用 Posix 标准，来减少这些差别。

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

### 信号量，互斥锁，条件变量差异

1. 互斥锁必须总是由给它上锁的线程解锁，信号量的挂出却不必由执行过它的等待操作的同一线程执行。
2. 互斥锁要么被锁住，要么被解开 （二值状态，类似二值信号量）
3. 既然信号量有个与之关联的状态（它的计数值），那么信号量的挂出操作总是被记住。然而当一个条件变量发送信号时，所有没有线程等待在该条件变量上，那么该信号将消失。

## 管道

管道有：匿名管道，有名管道（FIFO）
两者默认都是半双工， 一端用来写一端用来读。
```
int pipe(int fd[2])
int mkfifo(const char *pathname, mode_t mode)
```
+ 全双工使用socket函数

`int socketpair(int domain, int type, int protocol, int sv[2]);`


## 消息队列

消息队列的作用？

1. 程序解耦
2. 数据冗余，例如rabbitmq的ack机制，消息确认机制
3. 削峰能力
4. 可恢复性，就算系统中部分组件挂掉，消息在队列也不丢失，待组件恢复后继续处理消息。
5. 异步通信，如发红包，短信等流程丢入队列，可以优先级很低的去处理。
