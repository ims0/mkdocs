## Linux 中实时任务调度与优先级
Linux 系统中下面几个概念：
1. **实时**进程和**普通**进程的调度策略；

2. Linux 中**混乱的**进程优先级是如何计算的；

3. CPU**亲和性**的测试；

4. **多处理器**(SMP)遇到实时进程和普通进程的程序设计； 

5. 道哥的脑袋被门夹了一下的短路经历；

## 背景知识：Linux 调度策略
关于进程的 **调度策略**，不同的操作系统有不同的整体 **目标**，因此调度算法也就各不相同。

这需要根据进程的 **类型** (计算密集型？IO密集型？)、 **优先级** 等因素来进行选择。

对于 **Linux  x86** 平台来说，一般采用的是 **CFS**：**完全公平调度算法**。

之所以叫做完全公平，是因为操作系统以每个线程占用 **CPU** 的 **比率** 来进行动态的计算，操作系统希望每一个进程都能够 **平均** 的使用 **CPU** 这个资源，雨露均沾。

我们在创建一个线程的时候，**默认** 就是这个调度算法 **SCHED_OTHER** ，默认的 **优先级** 为 **0** 。

```
PS: 在 Linux 操作系统中，线程的内核对象与进程的内核对象(其实就是一些结构体变量)是很类似的，所以线程可以说是轻量级的进程。

在本文中，可以把线程约等于进程，有的地方也可能称为任务，在不同的语境下有一些不同的惯用说法。
```

可以这么理解：如果系统中一共有 **N** 个进程，那么每个进程会得到  **1/N**  的执行机会。每个进程执行 **一段时间** 之后，就被调出，换下一个进程执行。

如果这个 **N**  的数量太大了，导致每个进程刚开始执行时，分给它的时间就到了。如果这个时候就进行任务调度，那么系统的资源就耗费在 **进程上下文** 切换上去了。



因此，操作系统引入了 **最小粒度** ，也就是每个进程都有一个最小的执行时间保证，称作 **时间片** 。

除了 **SCHED_OTHER** 调度算法，**Linux** 系统还支持 **两种实时调度策略** ：

1. SCHED_FIFO：根据进程的优先级进行调度，一旦抢占到 CPU 则一直运行，直达自己主动放弃或被被更高优先级的进程抢占;

2. SCHED_RR：在 SCHED_FIFO 的基础上，加上了时间片的概念。当一个进程抢占到 CPU 之后，运行到一定的时间后，调度器会把这个进程放在 CPU 中，当前优先级进程队列的末尾，然后选择另一个相同优先级的进程来执行;

本文想测试的就是 **SCHED_FIFO** 与普通的 **SCHED_OTHER** 这两种调度策略混合的情况。

## 背景知识：Linux 线程优先级

在 **Linux** 系统中，优先级的管理显得 **比较混乱** ，先看下面这张图：

![avatar](linux_thread_sched_pic/pic1.png)

这张图表示的是 **内核中** 的优先级，分为两段。

前面的数值 **0-99** 是 **实时** 任务，后面的数值 **100-139** 是 **普通** 任务。

**数值越低，代表这个任务的优先级越高。**


再强调一下，以上是从 **内核** 角度来看的优先级。

好了，重点来了：

我们在 **应用层** 创建线程的时候，设置了一个优先级数值，这是从 **应用层角度** 来看的优先级数值。

但是 **内核并不会直接使用** 应用层设置的这个数值，而是经过了一定的 **运算** ，才得到内核中所使用的优先级数值( **0 ~ 139** )。

## 1. 对于实时任务
我们在创建线程的时候，可以通过下面这样的方式设置优先级数值( **0 ~ 99** )：

```
struct sched_param param;
param.__sched_priority = xxx;
```
当创建线程函数进入内核层面的时候，内核通过下面这个 **公式** 来计算真正的优先级数值：
```
kernel priority = 100 - 1 - param.__sched_priority
```
如果 **应用层** 传入数值 **0** ，那么在 **内核** 中优先级数值就是 **99** (100 - 1 - 0 = 99)，在所有实时任务中，它的优先级是 **最低** 的。

如果应用层传输数值 **99** ，那么在 **内核** 中优先级数值就是 **0** (100 - 1 - 99 = 0)，在所有实时任务中，它的优先级是 **最高** 的。

因此，从应用层的角度看，传输人优先级 **数值越大** ，线程的 **优先级就越高** ；数值 **越小** ，优先级就 **越低** 。

**与内核角度是完全相反的！**

## 2. 对于普通任务

调整普通任务的优先级，是通过 **nice** 值来实现的，内核中也有一个公式来把应用层传入的 **nice** 值，转成 **内核角度** 的优先级数值：
```
kernel prifoity = 100 + 20 + nice
```
**nice** 的合法数值是：**-20 ~ 19**。

如果 **应用层** 设置线程 **nice** 数值为 **-20**，那么在 **内核中** 优先级数值就是 **100(100 + 20 + (-20) = 100)** ，在所有的普通任务中，它的优先级是最高的。

如果应用层设置线程  **nice** 数值为 **19** ，那么在 **内核中** 优先级数值就是 **139(100 +20 +19 = 139)** ，在所有的普通任务中，它的优先级是最低的。

因此，从应用层的角度看，传输人优先级数值 **越小** ，线程的优先级就 **越高** ；数值 **越大** ，优先级就 **越低** 。

**与内核角度是完全相同的！**

背景知识交代清楚了，终于可以进行代码测试了！

## 测试代码说明

注意点：

1. `#define _GNU_SOURCE` 必须在 `#include <sched.h> `之前定义;
2. `#include <sched.h>` 必须在 `#include <pthread.h> `之前包含进来;
3. 这个顺序能够保证在后面设置继承的 CPU 亲和性时，CPU_SET, CEPU_ZERO这两个函数能被顺利定位到。


```
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>

// 用来打印当前的线程信息：调度策略是什么？优先级是多少？
void get_thread_info(const int thread_index)
{
    int policy;
    struct sched_param param;

    printf("\n====> thread_index = %d \n", thread_index);

    pthread_getschedparam(pthread_self(), &policy, &param);
    if (SCHED_OTHER == policy)
        printf("thread_index %d: SCHED_OTHER \n", thread_index);
    else if (SCHED_FIFO == policy)
        printf("thread_index %d: SCHED_FIFO \n", thread_index);
    else if (SCHED_RR == policy)
        printf("thread_index %d: SCHED_RR \n", thread_index);

    printf("thread_index %d: priority = %d \n", thread_index, param.sched_priority);
}

// 线程函数，
void *thread_routine(void *args)
{
    cpu_set_t mask;
    int cpus = sysconf(_SC_NPROCESSORS_CONF);
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
        printf("set thread affinity failed! \n");
    }
    // 参数是：线程索引号。四个线程，索引号从 1 到 4，打印信息中使用。
    int thread_index = *(int *)args;

    // 为了确保所有的线程都创建完毕，让线程睡眠1秒。
    sleep(1);

    // 打印一下线程相关信息：调度策略、优先级。
    get_thread_info(thread_index);

    long num = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 5000000; j++)
        {
            // 没什么意义，纯粹是模拟 CPU 密集计算。
            float f1 = ((i+1) * 345.45) * 12.3 * 45.6 / 78.9 / ((j+1) * 4567.89);
            float f2 = (i+1) * 12.3 * 45.6 / 78.9 * (j+1);
            float f3 = f1 / f2;
        }

        // 打印计数信息，为了能看到某个线程正在执行
        printf("thread_index %d: num = %ld \n", thread_index, num++);
    }

    // 线程执行结束
    printf("thread_index %d: exit \n", thread_index);
    return 0;
}

int main(void)
{
    // 一共创建四个线程：0和1-实时线程，2和3-普通线程(非实时)
    int thread_num = 4;

    // 分配的线程索引号，会传递给线程参数
    int index[4] = {1, 2, 3, 4};

    // 用来保存 4 个线程的 id 号
    pthread_t ppid[4];

    // 用来设置 2 个实时线程的属性：调度策略和优先级
    pthread_attr_t attr[2];
    struct sched_param param[2];

    // 实时线程，必须由 root 用户才能创建
    if (0 != getuid())
    {
        printf("Please run as root \n");
        exit(0);
    }

    // 创建 4 个线程
    for (int i = 0; i < thread_num; i++)
    {
        if (i <= 1)    // 前2个创建实时线程
        {
            // 初始化线程属性
            pthread_attr_init(&attr[i]);

            // 设置调度策略为：SCHED_FIFO
            pthread_attr_setschedpolicy(&attr[i], SCHED_FIFO);

            // 设置优先级为 51，52。
            param[i].__sched_priority = 51 + i;
            pthread_attr_setschedparam(&attr[i], &param[i]);

            // 设置线程属性：不要继承 main 线程的调度策略和优先级。
            pthread_attr_setinheritsched(&attr[i], PTHREAD_EXPLICIT_SCHED);

            // 创建线程
            pthread_create(&ppid[i], &attr[i],thread_routine, (void *)&index[i]);
        }
        else        // 后两个创建普通线程
        {
            pthread_create(&ppid[i], 0, thread_routine, (void *)&index[i]);
        }

    }

    // 等待 4 个线程执行结束
    for (int i = 0; i < 4; i++)
        pthread_join(ppid[i], 0);

    for (int i = 0; i < 2; i++)
        pthread_attr_destroy(&attr[i]);
}



```

https://mp.weixin.qq.com/s/9EDO1DL8pJKIXUXb8tR_cA