# listen

* [拆解 Linux 网络包发送过程](https://mp.weixin.qq.com/s?__biz=MjM5Njg5NDgwNA==&mid=2247485146&idx=1&sn=e5bfc79ba915df1f6a8b32b87ef0ef78&chksm=a6e307e191948ef748dc73a4b9a862a22ce1db806a486afce57475d4331d905827d6ca161711&scene=178&cur_album_id=1532487451997454337#rd)
* [图解 Linux网络包接收过程](https://mp.weixin.qq.com/s?__biz=MjM5Njg5NDgwNA==&mid=2247484058&idx=1&sn=a2621bc27c74b313528eefbc81ee8c0f&scene=21#wechat_redirect)

## <font color="#0099aa">TCP 半连接队列和全连接队列</font>

在 TCP 三次握手的时候，Linux 内核会维护两个队列，分别是：

* 全连接队列，也称 accepet 队列,长度是: <font color="#0099aa">min(backlog, somaxconn)</font>
* 半连接队列，也称 SYN 队列,长度是: <font color="#0099aa">min(backlog, somaxconn, tcp_max_syn_backlog) + 1 再上取整到 2 的幂次，但最小不能小于16</font>

* `/proc/sys/net/core/somaxconn (centos7:128,ubuntu20.04:4096)`
* `/proc/sys/net/ipv4/tcp_max_syn_backlog(def:128)`

![avator](listen_pic/listen.png)

## <font color="#0099ff">为什么服务端程序都需要先 listen 一下？</font>

### 一、创建 socket

服务器要做的第一件事就是先创建一个 socket。具体就是通过调用 socket 函数。当 socket 函数执行完毕后，在用户层视角我们是看到返回了一个文件描述符 fd。但在内核中其实是一套内核对象组合，大体结构如下。

![avator](listen_pic/socket_struct.png)
这里简单了解这个结构就行，后面我们在源码中看到函数指针调用的时候需要回头再来看它。

### 二、内核执行 listen
#### 2.1 listen 系统调用
[net/socket.c ](https://elixir.bootlin.com/linux/v4.3.6/source/net/socket.c)

```c linenums="1" hl_lines="7 9" 
//file: net/socket.c
SYSCALL_DEFINE2(listen, int, fd, int, backlog)
{
 sock = sockfd_lookup_light(fd, &err, &fput_needed);
 if (sock) {
  //获取内核参数 /proc/sys/net/core/somaxconn  4096
  somaxconn = sock_net(sock->sk)->core.sysctl_somaxconn;
  //对比
  if ((unsigned int)backlog > somaxconn)
    backlog = somaxconn;
  
  //调用协议栈注册的 listen 函数
  err = sock->ops->listen(sock, backlog);
  ......
}
```

用户态的 socket 文件描述符只是一个整数而已，所以该函数中第一行代码就是根据用户传入的文件描述符来查找到对应的 socket 内核对象。
再接着获取了系统里的 net.core.somaxconn 内核参数的值，和用户传入的 backlog 比较后取一个最小值传入到下一步中。
接着通过调用 sock->ops->listen 进入协议栈的 listen 函数。

#### 2.2 协议栈 listen与accept队列
这里我们需要用到第一节中的 socket 内核对象结构图了，通过它我们可以看出 sock->ops->listen 实际执行的是 inet_listen。

```c linenums="1" hl_lines="9 10"
//file: net/ipv4/af_inet.c
int inet_listen(struct socket *sock, int backlog)
{
 //还不是 listen 状态（尚未 listen 过）
 if (old_state != TCP_LISTEN) {
  //开始监听
  err = inet_csk_listen_start(sk, backlog);
 }
 //设置全连接队列长度
 sk->sk_max_ack_backlog = backlog;
}
```

在这里最底下这行，sk->sk_max_ack_backlog 是全连接队列的最大长度。
所以这里我们就知道了服务器的全连接队列长度是 min(backlog, net.core.somaxconn) 。

!!! note "note"
    如果你在线上遇到了全连接队列溢出的问题，想加大该队列长度，那么可能需要同时考虑 listen 时传入的 backlog 和 net.core.somaxconn。

再回过头看 inet_csk_listen_start 函数。

```c
//file: net/ipv4/inet_connection_sock.c
int inet_csk_listen_start(struct sock *sk, const int nr_table_entries)
{
 struct inet_connection_sock *icsk = inet_csk(sk);

 //icsk->icsk_accept_queue 是接收队列，详情见 2.3 节 
 //接收队列内核对象的申请和初始化，详情见 2.4节 
 int rc = reqsk_queue_alloc(&icsk->icsk_accept_queue, nr_table_entries);
 ......
}
```
在函数一开始，将 struct sock 对象强制转换成了 inet_connection_sock，名叫 icsk。

这里简单说下为什么可以这么强制转换，这是因为 inet_connection_sock 是包含 sock 的。tcp_sock、inet_connection_sock、inet_sock、sock 是逐层嵌套的关系，类似面向对象里的继承的概念。

![avator](listen_pic/socket_len.png)

在接下来的一行 reqsk_queue_alloc 中实际上包含了两件重要的事情。一是接收队列数据结构的定义。二是接收队列的申请和初始化。这两块都比较重要，我们分别在 2.3 节，和 2.4 节介绍。

#### 2.3 接收队列定义
icsk->icsk_accept_queue 定义在 inet_connection_sock 下，是一个 request_sock_queue 类型的对象。是内核用来接收客户端请求的主要数据结构。我们平时说的全连接队列、半连接队列全部都是在这个数据结构里实现的。


![avator](listen_pic/socket_listen.png)

```c
//file: include/net/inet_connection_sock.h
struct inet_connection_sock {
 /* inet_sock has to be the first member! */
 struct inet_sock   icsk_inet;
 struct request_sock_queue icsk_accept_queue;
 ......
}

```
我们再来查找到 request_sock_queue 的定义，如下。

```c
//file: include/net/request_sock.h
struct request_sock_queue {
 //全连接队列
 struct request_sock *rskq_accept_head;
 struct request_sock *rskq_accept_tail;

 //半连接队列
 struct listen_sock *listen_opt;
 ......
};
```

对于全连接队列来说，在它上面不需要进行复杂的查找工作，accept 的时候只是先进先出地接受就好了。所以全连接队列通过 rskq_accept_head 和 rskq_accept_tail 以链表的形式来管理。

和半连接队列相关的数据对象是 listen_opt，它是 listen_sock 类型的。

```c
//file: 
struct listen_sock {
 u8   max_qlen_log;
 u32   nr_table_entries;
 ......
 struct request_sock *syn_table[0];
};
```

因为服务器端需要在第三次握手时快速地查找出来第一次握手时留存的 request_sock 对象，所以其实是用了一个 hash 表来管理，就是 struct request_sock *syn_table[0]。max_qlen_log 和 nr_table_entries 都是和半连接队列的长度有关。

#### 2.4 接收队列申请和初始化
了解了全/半连接队列数据结构以后，让我们再回到 inet_csk_listen_start 函数中。它调用了 reqsk_queue_alloc 来申请和初始化 icsk_accept_queue 这个重要对象。

```
//file: net/ipv4/inet_connection_sock.c
int inet_csk_listen_start(struct sock *sk, const int nr_table_entries)
{
 ...
 int rc = reqsk_queue_alloc(&icsk->icsk_accept_queue, nr_table_entries);
 ...
}
```

在 reqsk_queue_alloc 这个函数中完成了接收队列 request_sock_queue 内核对象的创建和初始化。其中包括内存申请、半连接队列长度的计算、全连接队列头的初始化等等。

让我们进入它的源码：
```c
//file: net/core/request_sock.c
int reqsk_queue_alloc(struct request_sock_queue *queue,
        unsigned int nr_table_entries)
{
 size_t lopt_size = sizeof(struct listen_sock);
 struct listen_sock *lopt;

 //计算半连接队列的长度
 nr_table_entries = min_t(u32, nr_table_entries, sysctl_max_syn_backlog);
 nr_table_entries = ......

 //为 listen_sock 对象申请内存，这里包含了半连接队列
 lopt_size += nr_table_entries * sizeof(struct request_sock *);
 if (lopt_size > PAGE_SIZE)
  lopt = vzalloc(lopt_size);
 else
  lopt = kzalloc(lopt_size, GFP_KERNEL);

 //全连接队列头初始化
 queue->rskq_accept_head = NULL;

 //半连接队列设置
 lopt->nr_table_entries = nr_table_entries;
 queue->listen_opt = lopt;
 ......
}
```
开头定义了一个 struct listen_sock 指针。这个 listen_sock 就是我们平时经常说的半连接队列。

接下来计算半连接队列的长度。计算出来了实际大小以后，开始申请内存。最后将全连接队列头 queue->rskq_accept_head 设置成了 NULL，将半连接队列挂到了接收队列 queue 上。

!!! note "note"
    这里要注意一个细节，半连接队列上每个元素分配的是一个指针大小（sizeof(struct request_sock *)）。这其实是一个 Hash 表。真正的半连接用的 request_sock 对象是在握手过程中分配，计算完 Hash 值后挂到这个 Hash 表 上。

#### 2.5 半连接队列长度计算
在上一小节，我们提到 reqsk_queue_alloc 函数中计算了半连接队列的长度，由于这个有点小复杂，所以我们单独拉一个小节讨论这个。

file: 下面是v4.4之前的版本，4.4开始该函数做了修改.
[request_sock.c](https://elixir.bootlin.com/linux/v4.3.6/source/net/core/request_sock.c)
```c
/*
 *    Move a socket into listening state.
 */
int inet_csk_listen_start(struct sock *sk, const int nr_table_entries)
int sysctl_max_syn_backlog = 256;
EXPORT_SYMBOL(sysctl_max_syn_backlog);
int reqsk_queue_alloc(struct request_sock_queue *queue,
        unsigned int nr_table_entries)
{
 nr_table_entries = min_t(u32, nr_table_entries, sysctl_max_syn_backlog);
 nr_table_entries = max_t(u32, nr_table_entries, 8);
 /* roundup_pow_of_two - round the given value up to nearest power of two*/
 nr_table_entries = roundup_pow_of_two(nr_table_entries + 1);
 lopt_size += nr_table_entries * sizeof(struct request_sock *);
 ......
}
```

传进来的 nr_table_entries 在最初调用 reqsk_queue_alloc 的地方可以看到，它是内核参数 net.core.somaxconn 和用户调用 listen 时传入的 backlog 二者之间的较小值。

在这个 reqsk_queue_alloc 函数里，又将会完成三次的对比和计算。
min_t(u32, nr_table_entries, sysctl_max_syn_backlog) 这个是再次和 sysctl_max_syn_backlog 内核对象又取了一次最小值。
max_t(u32, nr_table_entries, 8) 这句保证 nr_table_entries 不能比 8 小，这是用来避免新手用户传入一个太小的值导致无法建立连接使用的。
roundup_pow_of_two(nr_table_entries + 1) 是用来上对齐到 2 的整数幂次的。

说到这儿，你可能已经开始头疼了。确实这样的描述是有点抽象。咱们换个方法，通过两个实际的 Case 来计算一下。

假设：某服务器上内核参数 net.core.somaxconn 为 128， net.ipv4.tcp_max_syn_backlog 为 8192。那么当用户 backlog 传入 5 时，半连接队列到底是多长呢？

和代码一样，我们还把计算分为四步，最终结果为 16。

min (backlog, somaxconn)  = min (5, 128) = 5
min (5, tcp_max_syn_backlog) = min (5, 8192) = 5
max (5, 8) = 8
roundup_pow_of_two (8 + 1) = 16
somaxconn 和 tcp_max_syn_backlog 保持不变，listen 时的 backlog 加大到 512，再算一遍，结果为 256。

min (backlog, somaxconn)  = min (512, 128) = 128
min (128, tcp_max_syn_backlog) = min (128, 8192) = 128
max (128, 8) = 128
roundup_pow_of_two (128 + 1) = 256

半连接队列长度总结: <font color="#0099aa">len=min(backlog, somaxconn, tcp_max_syn_backlog) + 1 再上取整到 2^n，len>=16。 </font>(kernel ver:3.10)

!!! note "note"
    如果你在线上遇到了半连接队列溢出的问题，想加大该队列长度，那么就需要同时考虑 somaxconn、backlog、和 tcp_max_syn_backlog 三个内核参数。

最后再说一点，为了提升比较性能，内核并没有直接记录半连接队列的长度。而是采用了一种晦涩的方法，只记录其幂次假设队列长度为 16，则记录 max_qlen_log 为 4 （2 ^4 = 16），假设队列长度为 256，则记录 max_qlen_log 为 8 （2 ^ 8 = 256）。大家只要知道这个东东就是为了提升性能的就行了。

## 总结一下

计算机系的学生就像背八股文一样记着服务器端 socket 程序流程：先 bind、再 listen、然后才能 accept。至于为什么需要先 listen 一下才可以 accpet，似乎我们很少去关注。

对 listen 源码的简单浏览，我们发现 listen 最主要的工作就是<font color="#0099ff">申请和初始化接收队列</font>(全连接队列,半连接队列)。
其中全连接队列是一个 **链表** ，而半连接队列由于需要快速的查找，所以使用的是一个 **哈希表** （其实半连接队列更准确的的叫法应该叫半连接哈希表）。

![avator](listen_pic/queue_st.png)


除此之外我们还有额外收获，我们还知道了内核是如何确定全/半连接队列的长度的。
```
/proc/sys/net/core/somaxconn
/proc/sys/net/ipv4/tcp_max_syn_backlog
```
1.**全连** 队列的长度

其最大长度是 listen 时传入的 **backlog** 和 **net.core.somaxconn** 之间较小的那个值。

2.**半连** 队列的长度

在 listen 的过程中，内核我们也看到了对于半连接队列来说，其最大长度是 min(backlog, somaxconn, tcp_max_syn_backlog) + 1 再上取整到 2 的幂次，但最小不能小于16。

https://mp.weixin.qq.com/s/hv2tmtVpxhVxr6X-RNWBsQ



### SYN Flood 攻击
故意大量不断发送伪造的SYN报文，那么服务器就会分配大量注定无用的资源，服务器能保存的半连接的数量是有限的！
所以当服务器受到大量攻击报文时，它就不能再接收正常的连接了，它是一种典型的DDoS攻击。
对于应对SYN 过多的问题，linux提供了几个TCP参数：tcp_syncookies、tcp_synack_retries、tcp_max_syn_backlog、tcp_abort_on_overflow 来调整应对。

### SYN Cookie
SYNcookie就是将连接信息编码在ISN(initialsequencenumber)中返回给客户端，这时server不需要将半连接保存在队列中，而是利用客户端随后发来的ACK带回的ISN还原连接信息，以完成连接的建立，避免了半连接队列被攻击SYN包填满。

linux 开关：`cat /proc/sys/net/ipv4/tcp_syncookies`

#### SYN cookies 算法
我们知道，TCP连接建立时，双方的起始报文序号是可以任意的。SYN cookies利用这一点，按照以下规则构造初始序列号：

* 设t为一个缓慢增长的时间戳(典型实现是每64s递增一次)
* 设m为客户端发送的SYN报文中的MSS选项值
* 设s是连接的元组信息(源IP,目的IP,源端口，目的端口)和t经过密码学运算后的Hash值，即s = hash(sip,dip,sport,dport,t)，s的结果取低 24 位

则初始序列号n为：

* 高 5 位为t mod 32
* 接下来3位为m的编码值
* 低 24 位为s

当客户端收到此SYN+ACK报文后，根据TCP标准，它会回复ACK报文，且报文中ack = n + 1，那么在服务器收到它时，将ack - 1就可以拿回当初发送的SYN+ACK报文中的序号了！服务器巧妙地通过这种方式间接保存了一部分SYN报文的信息。

接下来，服务器需要对ack - 1这个序号进行检查：

* 将高 5 位表示的t与当前之间比较，看其到达地时间是否能接受。
* 根据t和连接元组重新计算s，看是否和低 24 一致，若不一致，说明这个报文是被伪造的。
* 解码序号中隐藏的mss信息

到此，连接就可以顺利建立了。


