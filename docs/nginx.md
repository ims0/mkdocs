#

## 负载均衡
https://www.cnblogs.com/1214804270hacker/p/9325150.html

负载均衡用于从“upstream”模块定义的后端服务器列表中选取一台服务器接受用户的请求。
一个最基本的upstream模块是这样的，模块内的server是服务器列表：

```
#动态服务器组
upstream dynamic_zuoyu {
    server localhost:8080;  #tomcat 7.0
    server localhost:8081;  #tomcat 8.0
    server localhost:8082;  #tomcat 8.5
    server localhost:8083;  #tomcat 9.0
}
```

这就是最基本的负载均衡实例，但这不足以满足实际需求；目前Nginx服务器的upstream模块支持6种方式的分配：

name | intro
---  |---
轮询    |默认方式
weight  |  权重方式
ip_hash  |  依据ip分配方式
least_conn  |  最少连接方式
fair（第三方）  |  响应时间方式
url_hash（第三方）  |  依据URL分配方式 

在这里，只详细说明Nginx自带的负载均衡策略，第三方不多描述。
