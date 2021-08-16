## [epoll](https://mp.weixin.qq.com/s?__biz=Mzg3NTU3OTgxOA==&mid=2247492165&idx=1&sn=b7556601db1d4118ea9188945cb891aa&chksm=cf3df280f84a7b96a6247a59218bc30ac2487d14905924a2e64568bfe21762157595316b909c&mpshare=1&scene=23&srcid=0808zIlZVTSMDV57DFWC5Kjg&sharer_sharetime=1628663349817&sharer_shareid=04cc34d7c63f01f11762189fca8b134c#rd)


/fs/eventpoll.c
```c
SYSCALL_DEFINE1(epoll_create1, int, flags)
{
    return do_epoll_create(flags);
}

SYSCALL_DEFINE1(epoll_create, int, size)
{
    if (size <= 0)
        return -EINVAL;

    return do_epoll_create(0);
}

sysdeps/unix/sysv/linux/generic/epoll_create.c
 int
 epoll_create (int size)
 {
   if (size <= 0)
     {
       __set_errno (EINVAL);
       return -1;
     }

   return INLINE_SYSCALL (epoll_create1, 1, 0);
 }

```

## epoll 池原理
### 1. epoll 涉及的系统调用

epoll 的使用非常简单，只有下面 3 个系统调用。
```
epoll_create
epollctl
epollwait
```

就这？是的，就这么简单。

epollcreate 负责创建一个池子，一个监控和管理句柄 fd 的池子；  
epollctl 负责管理这个池子里的 fd 增、删、改；  
epollwait 就是负责打盹的，让出 CPU 调度，但是只要有“事”，立马会从这里唤醒；  

### 2. epoll 高效的原理


Linux 下，epoll 一直被吹爆，作为高并发 IO 实现的秘密武器。其中原理其实非常朴实：epoll 的实现几乎没有做任何无效功。 我们从使用的角度切入来一步步分析下。

首先，epoll 的第一步是创建一个池子。这个使用 epoll_create 来做：
```c
//原型：

int epoll_create(int size);
//示例：

epollfd = epoll_create(1024);
if (epollfd == -1) {
    perror("epoll_create");
    exit(EXIT_FAILURE);
}
```

这个池子对我们来说是黑盒，这个黑盒是用来装 fd 的，我们暂不纠结其中细节。我们拿到了一个 epollfd ，这个 epollfd 就能唯一代表这个 epoll 池。注意，这里又有一个细节：用户可以创建多个 epoll 池。

然后，我们就要往这个 epoll 池里放 fd 了，这就要用到 epoll_ctl 了

```c
原型：
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
示例：

if (epoll_ctl(epollfd, EPOLL_CTL_ADD, 11, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
}

```

上面，我们就把句柄 11 放到这个池子里了，op（EPOLL_CTL_ADD）表明操作是增加、修改、删除，event 结构体可以指定监听事件类型，可读、可写。

第一个跟高效相关的问题来了，添加 fd 进池子也就算了，如果是修改、删除呢？怎么做到快速？

这里就涉及到你怎么管理 fd 的数据结构了。

最常见的思路：用 list ，可以吗？功能上可以，但是性能上拉垮。list 的结构来管理元素，时间复杂度都太高 O(n)，每次要一次次遍历链表才能找到位置。池子越大，性能会越慢。

那有简单高效的数据结构吗？

有，红黑树。Linux 内核对于 epoll 池的内部实现就是用红黑树的结构体来管理这些注册进程来的句柄 fd。红黑树是一种平衡二叉树，时间复杂度为 O(log n)，就算这个池子就算不断的增删改，也能保持非常稳定的查找性能。



## epoll 之所以做到了高效，最关键的两点：

1. 内部管理 fd 使用了高效的红黑树结构管理，做到了增删改之后性能的优化和平衡；
1. epoll 池添加 fd 的时候，调用 file_operations->poll ，把这个 fd 就绪之后的回调路径安排好。通过事件通知的形式，做到最高效的运行；
1. epoll 池核心的两个数据结构：红黑树和就绪列表。红黑树是为了应对用户的增删改需求，就绪列表是 fd 事件就绪之后放置的特殊地点，epoll 池只需要遍历这个就绪链表，就能给用户返回所有已经就绪的 fd 数组；

![avtor](dataStruct_pic/epoll_struct.png)


## 思考

前面我们已经思考了很多知识点，有一些简单有趣的知识点，提示给读者朋友，这里只抛砖引玉。

问题：单核 CPU 能实现并行吗？

不行。

问题：单线程能实现高并发吗？

可以。

问题：那并发和并行的区别是？

一个看的是时间段内的执行情况，一个看的是时间时刻的执行情况。

问题：单线程如何做到高并发？

IO 多路复用呗，今天讲的 epoll 池就是了。

问题：单线程实现并发的有开源的例子吗？

redis，nginx 都是非常好的学习例子。当然还有我们 Golang 的 runtime 实现也尽显高并发的设计思想。

## 总结


1. IO 多路复用的原始实现很简单，就是一个 1 对多的服务模式，一个 loop 对应处理多个 fd ；
1. IO 多路复用想要做到真正的高效，必须要内核机制提供。因为 IO 的处理和完成是在内核，如果内核不帮忙，用户态的程序根本无法精确的抓到处理时机；
1. fd 记得要设置成非阻塞的哦，切记；
1. epoll 池通过高效的内部管理结构，并且结合操作系统提供的 poll 事件注册机制，实现了高效的 fd 事件管理，为高并发的 IO 处理提供了前提条件；
1. epoll 全名 eventpoll，在 Linux 内核下以一个文件系统模块的形式实现，所以有人常说 epoll 其实本身就是文件系统也是对的；
1. socketfd，eventfd，timerfd 这三种”文件“fd 实现了 poll 接口，所以网络 fd，事件fd，定时器fd 都可以使用 epoll_ctl 注册到池子里。我们最常见的就是网络fd的多路复用；
1. ext2，ext4，xfs 这种真正意义的文件系统反倒没有提供 poll 接口实现，所以不能用 epoll 池来管理其句柄。那文件就无法使用 epoll 机制了吗？不是的，有一个库叫做 libaio ，通过这个库我们可以间接的让文件使用 epoll 通知事件，以后详说，此处不表；
