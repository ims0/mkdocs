# IPC
## IPC形式汇总

1. 消息传递（管道，FIFO，消息队列）
2. 同步（互斥锁，条件变量，读写锁，信号量）
3. 共享内存区
4. 过程调用（Solaris 门， Sun RPC）

### 同步的方式区别与选择

posix.1 基本原理中声称，有了互斥锁和条件变量还提供信号量的原因如下：

*提供信号量的目的是提供一种进程间同步的方式，这些进程可能共享内存，也可能不共享内存，互斥锁和条件变量是作为线程间同步方式说明的，这些线程总是共享某个内存区，这两个是使用多年的同步范式，每组元语都特别适合特定的问题。*

虽然信号量是为了进程间同步而设置的，但是，信号量与互斥锁条件变量也能互换，根据实际问题选择合适的方式。



## UNIX 版本中systemV与posix

多数UNIX 系统或者源自 Berkeley，或者来自 System V
大多数厂家一开始采用 Posix 标准，来减少这些差别。

### [posix](https://linuxhint.com/posix-standard/)

Initially, POSIX was divided into multiple standards:

* POSIX.1: Core Services
* POSIX.1b: Real-time extensions
* POSIX.1c: Threads extensions
* POSIX.2: Shell and Utilities

### Basics of POSIX
POSIX.1-2008 standard deals with four major areas:

+ **Base Definition Volume:** General terms, concepts, and interfaces.
+ **Systems Interfaces Volume:** Definitions of system service functions and subroutines. Also, includes portability, error handling and error recovery.

+ **Shell and Utilities Volume:** Definition of interfaces of any application to command shells and common utility programs.
+ **Rationale Volume:** Contains information and history about added or discarded features and the reasonings of the decisions.
[systemV与posix对比](https://blog.csdn.net/firstlai/article/details/50705042)

### Posix IPC 名字规则

mq_open,sem_open,shm_open,的第一个参数都使用这样一个名字，它可以是真正的文件名也可以不是，为了便于移植，Posix必须以一个斜杠开头，并且不能再含有任何其它斜杠。

###  posix IPC 函数表

|type | 消息队列 | 信号量 | 共享内存|
|-- | -- | -- | --- |
|头文件|mqueue.h | semaphore.h | sys/mman.h |
|删建函数 | mq_open,mq_close, mq_unlink | sem_open, sem_close, sem_unlink; sem_init, sem_destory| shm_open, shm_unlink|
|控制IPC函数| mq_getattr, mq_setattr | | ftruncate, fstat|
| 操作函数 | mq_send, mq_receive, mq_notify | sem_init, sem_trywait, sem_post, sem_getvalue| mmap, munmap|

----

###  systemV IPC 函数表
|type | 消息队列 | 信号量 | 共享内存|
|-- | -- | -- | --- |
|头文件|<sys/msg.h> | <sys/sem.h> | sys/shm.h |
|创建打开IPC的函数 | msgget | semget| shmget|
|控制IPC函数       | msgctl| semctl | shmctl|
| 操作函数 | msgsnd, mqrcv | sem_op | shmat, shmdt|

----





## 消息队列

消息队列具有随内核的持续性，一个进程发送完成退出，另一个进程再去读取。
### Posix 消息队列 查看方式
其实消息队列就是一个可以让进程间交换数据的场所，而两个标准的消息队列最大的不同可能只是api 函数的不同，如system v 的系列函数是msgxxx，而posix 是mq_xxx。posix 消息队列也有一些对消息长度等的限制：
```
$ cat /proc/sys/fs/mqueue/msg_max 
10
$ cat /proc/sys/fs/mqueue/msgsize_max 
8192
$ cat /proc/sys/fs/mqueue/queues_max 
256
```
即一个消息队列最多能有10条消息，每条消息的最大长度为8192字节，一个系统最多能有256个消息队列。

还有一点是，在Linux上，posix 消息队列是以虚拟文件系统实现的，必须将其挂载到某个目录才能看见，如
```
# mkdir /dev/mqueue
# mount -t mqueue none /dev/mqueue
```
通过cat 命令查看消息队列的状态，假设mymq 是创建的一条消息队列的名字
```
$ cat /dev/mqueue/mymq
QSIZE:129     NOTIFY:2    SIGNO:0    NOTIFY_PID:8260
```
+ QSIZE：消息队列所有消息的数据长度
+ NOTIFY_PID：某个进程使用mq_notify 注册了消息到达异步通知事件，即此进程的pid
+ NOTIFY：通知方式： 0 is SIGEV_SIGNAL; 1 is SIGEV_NONE; and 2 is SIGEV_THREAD.
+ SIGNO：当以信号方式通知的时候，表示信号的编号.

### Posix 消息队列函数签名

#### mq_open 
- open a message queue

```
mqd_t mq_open(const char *name, int oflag);
mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

c语言没有函数重载，这里用的是变参实现两种形式，实际形式如下
mqd_t mq_open (const char *__name, int __oflag, ...)
/* Establish connection between a process and a message queue NAME and
   return message queue descriptor or (mqd_t) -1 on error.  OFLAG determines
   the type of access used.  If O_CREAT is on OFLAG, the third argument is
   taken as a `mode_t', the mode of the created message queue, and the fourth
   argument is taken as `struct mq_attr *', pointer to message queue
   attributes.  If the fourth argument is NULL, default attributes are
   used.  */
```
第二个参数oflag必须被指定下面三个之一:
```
O_RDONLY  :接收消息
O_WRONLY  :发送消息
O_RDWR    :收发消息
```
下面是oflag附加参数，|到oflag
```
O_CLOEXEC   :close-on-exec
O_CREAT     :Create  the message queue if it does not exist.
O_EXCL      :if name already exists, then fail with the error EEXIST.
O_NONBLOCK  :Open the queue in nonblocking mode.
```
第三个参数 mode, 设置读写执行权限

```
#include <sys/stat.h>
S_IRUSR   : 用户读
S_IWUSR   : 用户写
S_IRGRP   : 组成员读
S_IWGRP   : 组成员写
S_IROTH   : other读
S_IWOTH   : other写
```

#### mq_send
```
#include <mqueue.h>
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

#include <time.h>
#include <mqueue.h>
int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio,
                     const struct timespec *abs_timeout);
```

+ msg_len  
    this length must be less than or equal to the queue's  mq_msgsize  attribute.  Zero-length messages are allowed.
+ msg_prio  
    argument is a nonnegative integer that specifies the priority of this message.

如果队列满的话，mqsend将会阻塞，如果设置为非阻塞，则立即返回 with the error EAGAIN.

mq_timedsend() behaves just like mq_send(),如过没有使用 O_NONBLOCK 这个函数将会阻塞直到时间参数到期，如果队列满，且时间到期,mq_timedsend立即返回

#### mq_receive
```
#include <mqueue.h>
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr,
                      size_t msg_len, unsigned int *msg_prio);

#include <time.h>
#include <mqueue.h>
ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio,
                          const struct timespec *abs_timeout);
```
+ 返回优先级最高到的最早的消息，
+ msg_prio 参数如果非NULL，则返回消息的优先级
+ 函数默认阻塞直到有消息到达，如过设置非阻塞，则立即返回with the error EAGAIN
+ mq_timedreceive() 如果时间到期没有消息到达，则返回


#### mq_getattr/mq_setattr
```
#include <mqueue.h>
int mq_getattr(mqd_t mqdes, struct mq_attr *attr);
int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr);
```

mq_getattr() returns an mq_attr structure in the buffer pointed by attr.  This structure is defined as:

```
   struct mq_attr {
       long mq_flags;       /* Message queue flags. mq_setattr*/
       long mq_maxmsg;      /* Maximum number of messages. mq_open use */
       long mq_msgsize;     /* Maximum message size. mq_open use  */
       long mq_curmsgs;     /* Number of messages currently queued. mq_getatt*/
   };
 Only the mq_maxmsg and mq_msgsize fields are employed when calling mq_open(); 
 the values in the remaining fields are ignored.

```
+ mq_setattr唯一可以修改的字段是mq_flags，可选为阻塞或者非阻塞，其它属性在消息创建时指定，不能修改。
+ mq_open 可以识别的字段是mq_maxmsg,mq_msgsize，
1. attr.mq_maxmsg 不能超过文件 /proc/sys/fs/mqueue/msg_max 中的数值；
2. attr.mq_msgsize不能超过 /proc/sys/fs/mqueue/msgsize_max 的数值；
3. 在POSIX消息队列中 msg_max 默认为 10 ，msgsize_max 默认为8192 ，否则会报错！！！
+ 如果 oldattr 不为NULL则返回旧属性类似mq_getattr()

####  mq_close
- close a message queue descriptor
关闭队列指针，程序结束会自动关闭
```
       int mq_close(mqd_t mqdes);
```

#### mq_unlink
- remove a message queue
当队列引用计数为0的时候，删除系统中的
```
int mq_unlink(const char *name);
```
#### mq_notify

System V消息队列的问题之一是无法通知一个进程何时在某个队列中放置了一个消息。采用轮询（poling），是对CPU时间的一种浪费。Posix消息队列容许 异步事件通知，以告知何时有一个消息放置到某个空消息队列中。该通知有两种方式：当一个消息被放置某个空队列时，样么产生一个信号来通知，样么通过创建一个线程来执行一个特定程序，来完成消息到来时的该做的事情。

```
int mq_notify(mqd_t mqdes, const struct sigevent* notification);
union sigval{
int     sival_int;          /*integer value*/
void    *sival_ptr;         /*pointer value*/
};
 
struct sigevent{
int     sigev_notify;       /*SIGEV_{NONE, SIGNAL, THREAD}*/
int     sigev_signo;        /*signal number if SIGEV_SIGNAL*/
 
union sigval    sigev_value;//sigev_notify_function指定函数的参数由该变量指定
```
该函数为指定队列建立或删除异步事件通知

+ (1).如果notification参数为非空，那么当前进程希望在有一个消息到达所指定的先前为空的对列时得到通知。
+ (2).如果notification参数为空，而且当前进程被注册为接收指定队列的通知，那么已存在的注册将被撤销。
+ (3).任意时刻只有一个进程可以被注册为接收某个给定队列的通知。
+ (4).当有一个消息到达先前为空的消息队列，而且已有一个进程被注册为接收该队列的通知时，只有在没有任何线程阻塞在该队列的mq_receive调用中的前提下，通知才会发出。即说明，在mq_receive调用中的阻塞比任何通知的注册都优先。
+ (5).当前通知被发送给它的注册进程时，其注册几倍撤销。该进程必须再次调用mq_notify以重新注册。



### SystemV 消息队列函数签名
#### msgget
returns the System V message queue identifier associated with the value of the key argument.  
```
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
```
+ key 可以是IPC_PRIVATE ，或者是ftok(const char *pathname, int proj_id)返回值
+ msgflg 是读写权限值的组合，它可以与`IPC_CREAT and IPC_EXCL`按位或。

#### msgsnd

```
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```
+ msgp 是一个结构体指针，第一个字段是long类型的消息类型，后续字段自己定义 
```
typedef struct{
    long msgtype;
    char userdata[SIZE];
}Message;

```
+ msgsz 以字节为单位指定待发消息的大小,上述消息长度可以表示为：sizeof(Message) - sizeof(long)，

+ msgflg 参数可以是0，也可以是IPC_NOWAIT,IPC_NOWAIT 标志使得msgsnd调用非阻塞（如果没有存消息的空间，立马返回）

#### msgrcv
```
size_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtype, int msgflg);
```
+ msgp 参数指定所接收消息的存放位置，和msgsnd一样，指向紧挨着真正数据前的长整型字段。
+ msgsz 指向缓冲区大小，不包括长整型字段。
+ msgtype 指定希望读出消息的类型

+ if msgtype == 0 ,返回最早的消息
+ if msgtype > 0, 返回其类型值为type的第一个消息。
+ if msgtype < 0, 返回其类型值<= msgtype参数的绝对值的消息中类型值最小的第一个消息。

#### msgctl

```
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```
+ cmd 可取值：
`IPC_STAT,IPC_SET,IPC_RMID,IPC_INFO,MSG_INFO,MSG_STAT,MSG_STAT_ANY`

+ buf 参数的结构

```
struct msqid_ds {
   struct ipc_perm msg_perm;     /* Ownership and permissions */
   time_t          msg_stime;    /* Time of last msgsnd(2) */
   time_t          msg_rtime;    /* Time of last msgrcv(2) */
   time_t          msg_ctime;    /* Time of last change */
   unsigned long   __msg_cbytes; /* Current number of bytes in queue (nonstandard) */
   msgqnum_t       msg_qnum;     /* Current number of messages in queue */
   msglen_t        msg_qbytes;   /* Maximum number of bytes allowed in queue */
   pid_t           msg_lspid;    /* PID of last msgsnd(2) */
   pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
};
```


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

## 互斥锁

互斥的概念
在多线程编程中，引入了对象互斥锁的概念，来保证共享数据操作的完整性。 每个对象都对应于一个可称为" 互斥锁" 的标记，这个标记用来保证在任一时刻， 只能有一个线程访问该对象。

互斥锁操作
互斥锁也可以叫线程锁，接下来说说互斥锁的的使用方法。

对互斥锁进行操作的函数，常用的有如下几个：
```
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout);
```
对线程锁进行操作的函数有很多，还包括许多线程锁属性的操作函数， 不过一般来说，对于并不复杂的情况， 只需要使用创建、获取锁、释放锁、删除锁这几个就足够了。


### 非阻塞调用
如果不想阻塞，而是想尝试获取一下，如果锁被占用咱就不用，如果没被占用那就用， 这该怎么实现呢？可以使用 pthread_mutex_trylock() 函数。 这个函数和 pthread_mutex_lock() 用法一样，只不过当请求的锁正在被占用的时候， 不会进入阻塞状态，而是立刻返回，并返回一个错误代码 EBUSY，意思是说， 有其它线程正在使用这个锁。
```
int err = pthread_mutex_trylock(&mtx);
if(0 != err) {
    if(EBUSY == err) {
        //The mutex could not be acquired because it was already locked.
    }
}
```
### 超时调用
如果不想不断的调用 pthread_mutex_trylock() 来测试互斥锁是否可用， 而是想阻塞调用，但是增加一个超时时间呢，那么可以使用 pthread_mutex_timedlock() 来解决， 其调用方式如下：
```
struct timespec abs_timeout;
abs_timeout.tv_sec = time(NULL) + 1;
abs_timeout.tv_nsec = 0;

int err = pthread_mutex_timedlock(&mtx, &abs_timeout);
if(0 != err) {
    if(ETIMEDOUT == err) {
        //The mutex could not be locked before the specified timeout expired.
    }
}
``` 
上面代码的意思是，阻塞等待线程锁，但是只等1秒钟，一秒钟后如果还没拿到锁的话， 那就返回，并返回一个错误代码 ETIMEDOUT，意思是超时了。


## 条件变量
### 初始化
1. 条件变量和互斥锁一样，都有静态动态两种创建方式，静态方式使用PTHREAD_COND_INITIALIZER常量，如下：
`pthread_cond_t cond=PTHREAD_COND_INITIALIZER`
2. 动态方式调用pthread_cond_init()函数，API定义如下：
`int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr)`

### 等待
```
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
```

   等待条件有两种方式：条件等待pthread_cond_wait()和计时等待pthread_cond_timedwait()，其中计时等待方式如果在给定时刻前条件没有满足，则返回ETIMEDOUT，结束等待，其中abstime以与time()系统调用相同意义的绝对时间形式出现，0表示格林尼治时间1970年1月1日0时0分0秒。

  无论哪种等待方式，都必须和一个互斥锁配合，以防止多个线程同时请求pthread_cond_wait()（或pthread_cond_timedwait()，下同）的竞争条件（Race Condition）。mutex互斥锁必须是普通锁（PTHREAD_MUTEX_TIMED_NP）或者适应锁（PTHREAD_MUTEX_ADAPTIVE_NP），且在调用pthread_cond_wait()前必须由本线程加锁（pthread_mutex_lock()），而在更新条件等待队列以前，mutex保持锁定状态，并在线程挂起进入等待前解锁。在条件满足从而离开pthread_cond_wait()之前，mutex将被重新加锁，以与进入pthread_cond_wait()前的加锁动作对应。阻塞时处于解锁状态。

### 激发

条件有两种形式，pthread_cond_signal()激活一个等待该条件的线程，存在多个等待线程时按入队顺序激活其中一个；而pthread_cond_broadcast()则激活所有等待线程。

## 共享内存 IPC

由于数据传递不涉及内核，所以共享内存是IPC中速度最快的，存取数据需要各种形式的同步:互斥锁，条件变量，读写锁，记录锁，信号量。

###  posix ShareMem

mmap,munmap,msync

mmap函数把一个文件或一个Posix共享内存区对象映射到调用进程的地址空间，使用函数三个目的:

1. 使用普通文件以提供内存映射I/O
2. 使用特殊文件以提供内存匿名映射
3. 使用shm_open以提供无亲缘关系进程间的Posix共享内存区

`void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)`

Posix共享内存构筑在mmap函数之上，首先指定一个Posix IPC名字来调用shm_open(),取得描述符后，使用mmap把它映射到内存，类似于内存映射文件。
如果用内存映射文件的形式来实现Posix共享内存，只需要把shm_open()调用open(),shm_unlink调用unlink实现。

### posix 共享内存函数签名
```
#include <sys/mman.h>
/* Open shared memory segment.  */
extern int shm_open (const char *__name, int __oflag, mode_t __mode);

int ftruncate (int __fd, __off_t __length) __THROW __wur;

extern void *mmap (void *__addr, size_t __len, int __prot,
		   int __flags, int __fd, __off_t __offset) __THROW;
           
/* Remove shared memory segment.  */
extern int shm_unlink (const char *__name); 
```


```
  // O_TRUNC:若文件存在，则长度被截为0，属性不变
  int oflag = O_RDWR | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR;
  int fd = shm_open(px_name, oflag, mode);
  if (fd == -1) {
    fprintf(stderr, "shm_open: %s\n", strerror(errno));
    return 1;
  }
  printf("fd: %d \n", fd);
  ftruncate(fd, sizeof(people) * 10);

  people *p_map = (people *)mmap(NULL, sizeof(people) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p_map == (void *)-1) {
    fprintf(stderr, "mmap: %s\n", strerror(errno));
    return 1;
  }
  close(fd);

```

### SystemV共享内存

[共享内存使用常见陷阱与分析](https://blog.csdn.net/killmice/article/details/41516399)

#### 共享内存系统范围最大值
在一个linux服务器上，共享内存的总体大小是有限制的，这个大小通过SHMMAX参数来定义（以字节为单位），您可以通过执行以下命令来确定 SHMMAX 的值：

` cat /proc/sys/kernel/shmmax`
linux 64 default: 18446744073692774399
#### 系统范围内共享内存段的最大数量
` cat /proc/sys/kernel/shmmni`
linux 64 default: 4096 

#### SHMALL

最后，我们来看 SHMALL 共享内存内核参数。该参数控制着系统一次可以使用的共享内存总量（以页为单位）。简言之，该参数的值始终应该至少为：
ceil(SHMMAX/PAGE_SIZE)

`cat /proc/sys/kernel/shmall` ubuntu64 default: 18446744073692774399



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


