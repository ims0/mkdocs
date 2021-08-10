##

/fs/eventpoll.c
```
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
