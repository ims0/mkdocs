

!!! note "tip"
    demo

## <font color="#0099ff"> hello </font>


```c linenums="1" hl_lines="7 9" 
//file: net/socket.c
SYSCALL_DEFINE2(listen, int, fd, int, backlog)
{
 sock = sockfd_lookup_light(fd, &err, &fput_needed);
 if (sock) {  // (1)
  //获取内核参数 /proc/sys/net/core/somaxconn  4096
  somaxconn = sock_net(sock->sk)->core.sysctl_somaxconn;
  //对比
  if ((unsigned int)backlog > somaxconn)
    backlog = somaxconn;
  
  //调用协议栈注册的 listen 函数
  err = sock->ops->listen(sock, backlog);
  ......
}

1. ...
```
``` mermaid
classDiagram
  Person <|-- Student
  Person <|-- Professor
  Person : +String name
  Person : +String phoneNumber
  Person : +String emailAddress
  Person: +purchaseParkingPass()
  Address "1" <-- "0..1" Person:lives at
  class Student{
    +int studentNumber
    +int averageMark
    +isEligibleToEnrol()
    +getSeminarsTaken()
  }
  class Professor{
    +int salary
  }
  class Address{
    +String street
    +String city
    +String state
    +int postalCode
    +String country
    -validate()
    +outputAsLabel()  
  }
```



```c linenums="1"  

--8<-- "docs/code/avl.cpp"
```



* [OI]( https://oi-wiki.org/)
* [mkdocs-material]( https://squidfunk.github.io/mkdocs-material/)
* [基于mkdocs-material]( https://cyent.github.io/markdown-with-mkdocs-material/install/local/)
