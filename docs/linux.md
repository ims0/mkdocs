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

## ps

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

### PROCESS STATE CODES         top
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

## netstat

## lsof
## nc

https://www.cnblogs.com/bakari/p/10898604.html

## tcpdump

https://www.cnblogs.com/bakari/p/10748721.html

## top
## df
## fdisk
## ifconfig
## uname
## kill
## ipcs
