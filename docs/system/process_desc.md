
#

## Linux 对进程的描述

进程是操作系统种调度的实体，对进程拥有资源的描述称为进程控制块（PCB, Process Contrl Block）

## 通过 task_struct 描述进程

内核里，通过 task_struct 结构体来描述一个进程，称为进程描述符 (process descriptor)，它保存着支撑一个进程正常运行的所有信息。task_struct 结构体内容太多，这里只列出部分成员变量，感兴趣的读者可以去源码 include/linux/sched.h头文件查看。
```
struct task_struct {

#ifdef CONFIG_THREAD_INFO_IN_TASK
  /*
   * For reasons of header soup (see current_thread_info()), this
   * must be the first element of task_struct.
   */
  struct thread_info        thread_info;
#endif
  volatile long state;
  void *stack;
  ......
  struct mm_struct *mm;
  ......
  pid_t pid;
  ......
  struct task_struct *parent;
  ......
  char comm[TASK_COMM_LEN];
  ......
  struct files_struct *files;
  ......
  struct signal_struct *signal;
}
```

task_struct 中的主要信息分类：

1. 标示符：描述本进程的唯一标识符 pid，用来区别其他进程。
2. 状态：任务状态，退出代码，退出信号等
3. 优先级：相对于其他进程的优先级
4. 程序计数器：程序中即将被执行的下一条指令的地址
5. 内存指针：包括程序代码和进程相关数据的指针，还有和其他进程共享的内存块的指针
6. 上下文数据：进程执行时处理器的寄存器中的数据
7. I/O状态信息：包括显示的I/O请求，分配的进程I/O设备和进程使用的文件列表
8. 记账信息：可能包括处理器时间总和，使用的时钟总和，时间限制，记帐号等  

---

* struct thread_info thread_info: 进程被调度执行的信息
* volatile long   state：-1是不运行的，=0是运行状态，>0是停止状态。下面是几个比较重要的进程状态以及它们之间的转换流程。

![avatar](process_pic/pic1.png)

* void    *stack：指向内核栈的指针，内核通过 dup_task_struct 为每个进程都分配内核栈空间，并记录在此。
* struct mm_struct *mm: 与进程地址空间相关的信息。

![avatar](process_pic/pic2.png)

* pid_t pid: 进程标识符
* char comm[TASK_COMM_LEN]: 进程的名称
* struct files_struct *files: 打开的文件表
* struct signal_struct *signal: 信号处理相关

## task_struct, thread_info 和内核栈 sp 的关系

接着看下 thread_info 结构：


```
struct thread_info {
        unsigned long           flags;          /* low level flags */
        mm_segment_t            addr_limit;     /* address limit */
#ifdef CONFIG_ARM64_SW_TTBR0_PAN
        u64                     ttbr0;          /* saved TTBR0_EL1 */
#endif
        union {
                u64             preempt_count;  /* 0 => preemptible, <0 => bug */
                struct {
#ifdef CONFIG_CPU_BIG_ENDIAN
                        u32     need_resched;
                        u32     count;
#else
                        u32     count;
                        u32     need_resched;
#endif
                } preempt;
        };
#ifdef CONFIG_SHADOW_CALL_STACK
        void                    *scs_base;
        void                    *scs_sp;
#endif
};

```

接着再来看下内核栈的定义:

```
union thread_union {
#ifndef CONFIG_ARCH_TASK_STRUCT_ON_STACK
        struct task_struct task;
#endif
#ifndef CONFIG_THREAD_INFO_IN_TASK
        struct thread_info thread_info;
#endif
        unsigned long stack[THREAD_SIZE/sizeof(long)];
};

```

当 CONFIG_THREAD_INFO_IN_TASK 这个配置打开的时候，则 thread_union 结构中只存在 stask 成员了。

内核在启动的时候会在 head.S 里通过 __primary_switched 来做内核栈的初始化：

```
SYM_FUNC_START_LOCAL(__primary_switched)
        adrp    x4, init_thread_union
        add     sp, x4, #THREAD_SIZE
        adr_l   x5, init_task
        msr     sp_el0, x5                      // Save thread_info
```

将 init_thread_union 的地址保存到 x4，然后偏移 THREAD_SIZE 栈大小，用于初始化 sp。将 init_task 进程描述符地址赋值给 x5，并保存到 sp_el0。

下面再看下 init_thread_union 和 init_task 的定义：
```
#include/linux/sched/task.h
extern union thread_union init_thread_union;

#init/init_task.c
struct task_struct init_task __aligned(L1_CACHE_BYTES) =
{
#ifdef CONFIG_THREAD_INFO_IN_TASK
        .thread_info    = INIT_THREAD_INFO(init_task),
        .stack_refcount = REFCOUNT_INIT(1),
#endif
.....
};

```

 故这三者的关系可以通过下图描述：

![avatar](process_pic/pic3.png)

## 如何获取当前进程

 内核中经常通过 current 宏来获得当前进程对应的 struct task_sturct 结构，我们借助 current，结合上面介绍的内容，看下具体的实现。


```
static __always_inline struct task_struct *get_current(void)
{
    unsigned long sp_el0;
 
    asm ("mrs %0, sp_el0" : "=r" (sp_el0));
 
    return (struct task_struct *)sp_el0;
}
 
#define current get_current()
```

代码比较简单，可以看出通过读取用户空间栈指针寄存器 sp_el0 的值，然后将此值强转成 task_struct 结构就可以获得当前进程。（sp_el0 里存放的是 init_task，即 thread_info 地址，thread_info 又是在 task_sturct 的开始处，从而找到当前进程。）


[src url](https://mp.weixin.qq.com/s/UccJLhVUlsTNB8xdzmJ90g)
