
## 六，数据库的基本操作

### 创建用户与授权

+ username：你将创建的用户名
+ host：指定该用户在哪个主机上可以登陆，如果是本地用户可用localhost，如果想让该用户可以从任意远程主机登陆，可以使用通配符%
+ password：该用户的登陆密码，密码可以为空，如果为空则该用户可以不需要密码登陆服务器

```sql
CREATE USER 'username'@'host' IDENTIFIED BY 'password';

CREATE USER 'dog'@'localhost' IDENTIFIED BY '123456';
CREATE USER 'pig'@'192.168.1.101_' IDENDIFIED BY '123456';
CREATE USER 'pig'@'%' IDENTIFIED BY '123456';
CREATE USER 'pig'@'%' IDENTIFIED BY '';
CREATE USER 'pig'@'%';
```
授权

+ privileges：用户的操作权限，如SELECT，INSERT，UPDATE等，如果要授予所的权限则使用ALL
+ databasename：数据库名
+ tablename：表名，如果要授予该用户对所有数据库和表的相应操作权限则可用*表示，如*.*

```sql
GRANT privileges ON databasename.tablename TO 'username'@'host'
GRANT SELECT, INSERT ON test.user TO 'pig'@'%';
GRANT ALL ON *.* TO 'pig'@'%';

CREATE USER 'remote'@'%' IDENTIFIED BY '123456';
GRANT ALL  ON test_db.* TO 'repl'@'%';

ALTER USER 'repl'@'%' IDENTIFIED WITH mysql_native_password BY '123456';
#刷新系统权限表的配置
FLUSH PRIVILEGES;

```
### 修改用户密码
`update user set authentication_string=password('123456') where user='ims';`

### 创建数据库

`CREATE DATABASE 数据库名;`

### 创建表

```sql
CREATE TABLE test(
    id INT NOT NULL AUTO_INCREMENT  PRIMARY KEY ,
    title VARCHAR(100) NOT NULL,
    author CHAR(20) NOT NULL,
    insertime timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT '添加时间',
    updatetime timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间'
    )DEFAULT CHARSET=utf8;
```

+ 显示表信息命令: `show create table test`, 类似`desc`,只是前者按命令格式。
+ 修改表引擎的命令 : `alter table test engine = innodb;` 

InnoDB 主键使用的是聚簇索引，MyISAM 不管是主键索引，还是二级索引使用的都是非聚簇索引

### 删除表
`DROP TABLE table_name ;`

### 插入数据

```sql
INSERT INTO table_name ( field1, field2,...fieldN ) VALUES ( value1, value2,...valueN );
```

### 删除数据
```sql
DELETE FROM table_name [WHERE Clause]
DELETE FROM runoob_tbl WHERE runoob_id=3;
```

### 更新数据

```sql
UPDATE table_name SET field1=new-value1, field2=new-value2
[WHERE Clause]
```

### 修改表字段

+ 删除字段 i
` ALTER TABLE testalter_tbl  DROP i;`
+ 添加字段
`ALTER TABLE testalter_tbl ADD i INT;`

+ 修改字段类型
`ALTER TABLE testalter_tbl MODIFY c CHAR(10);`
+ 修改字段类型及名称
`ALTER TABLE testalter_tbl CHANGE i j BIGINT;`

+ 修改表名
`ALTER TABLE testalter_tbl RENAME TO alter_tbl;`

### order
```sql
select *from test order by title;
```

### join

JOIN 按照功能大致分为如下三类：

INNER JOIN（内连接,或等值连接）：获取两个表中字段匹配关系的记录。
LEFT JOIN（左连接）：获取左表所有记录，即使右表没有对应匹配的记录。
RIGHT JOIN（右连接）： 与 LEFT JOIN 相反，用于获取右表所有记录，即使左表没有对应匹配的记录。

```sql
select * from booklist a JOIN author b on a.author=b.author;
select * from booklist a left JOIN author b on a.author=b.author;
select * from booklist a right JOIN author b on a.author=b.author;

mysql> select * from author;
+--------------+-------+
| author       | count |
+--------------+-------+
| 菜鸟教程     |    20 |
| RUNOOB.COM   |    30 |
| google       |    40 |
| mengso       |    50 |
+--------------+-------+
4 rows in set (0.00 sec)

mysql> select * from booklist;
+----+----------------+--------------+---------------------+
| id | title          | author       | submission_date     |
+----+----------------+--------------+---------------------+
|  1 |  学习 PHP      | 菜鸟教程     | 2021-08-17 23:28:59 |
|  2 |  学习 mysql    | 菜鸟教程     | 2021-08-17 23:05:59 |
|  3 |  学习 java     | RUNOOB.COM   | 2021-08-17 23:25:59 |
|  4 |  学习 python   | RUNOOB.COM   | 2021-08-17 23:28:59 |
|  5 |  学习 c        | fk           | 2021-08-17 23:31:37 |
|  6 |  学习 c++      | fk           | 2021-08-17 23:46:53 |
+----+----------------+--------------+---------------------+
6 rows in set (0.00 sec)

mysql> select * from booklist a JOIN author b on a.author=b.author;
+----+----------------+--------------+---------------------+--------------+-------+
| id | title          | author       | submission_date     | author       | count |
+----+----------------+--------------+---------------------+--------------+-------+
|  1 |  学习 PHP      | 菜鸟教程     | 2021-08-17 23:28:59 | 菜鸟教程     |    20 |
|  2 |  学习 mysql    | 菜鸟教程     | 2021-08-17 23:05:59 | 菜鸟教程     |    20 |
|  3 |  学习 java     | RUNOOB.COM   | 2021-08-17 23:25:59 | RUNOOB.COM   |    30 |
|  4 |  学习 python   | RUNOOB.COM   | 2021-08-17 23:28:59 | RUNOOB.COM   |    30 |
+----+----------------+--------------+---------------------+--------------+-------+
4 rows in set (0.00 sec)

mysql> select * from booklist a left JOIN author b on a.author=b.author;
+----+----------------+--------------+---------------------+--------------+-------+
| id | title          | author       | submission_date     | author       | count |
+----+----------------+--------------+---------------------+--------------+-------+
|  1 |  学习 PHP      | 菜鸟教程     | 2021-08-17 23:28:59 | 菜鸟教程     |    20 |
|  2 |  学习 mysql    | 菜鸟教程     | 2021-08-17 23:05:59 | 菜鸟教程     |    20 |
|  3 |  学习 java     | RUNOOB.COM   | 2021-08-17 23:25:59 | RUNOOB.COM   |    30 |
|  4 |  学习 python   | RUNOOB.COM   | 2021-08-17 23:28:59 | RUNOOB.COM   |    30 |
|  5 |  学习 c        | fk           | 2021-08-17 23:31:37 | NULL         |  NULL |
|  6 |  学习 c++      | fk           | 2021-08-17 23:46:53 | NULL         |  NULL |
+----+----------------+--------------+---------------------+--------------+-------+
6 rows in set (0.00 sec)

mysql> select * from booklist a right JOIN author b on a.author=b.author;
+------+----------------+--------------+---------------------+--------------+-------+
| id   | title          | author       | submission_date     | author       | count |
+------+----------------+--------------+---------------------+--------------+-------+
|    2 |  学习 mysql    | 菜鸟教程     | 2021-08-17 23:05:59 | 菜鸟教程     |    20 |
|    1 |  学习 PHP      | 菜鸟教程     | 2021-08-17 23:28:59 | 菜鸟教程     |    20 |
|    4 |  学习 python   | RUNOOB.COM   | 2021-08-17 23:28:59 | RUNOOB.COM   |    30 |
|    3 |  学习 java     | RUNOOB.COM   | 2021-08-17 23:25:59 | RUNOOB.COM   |    30 |
| NULL | NULL           | NULL         | NULL                | google       |    40 |
| NULL | NULL           | NULL         | NULL                | mengso       |    50 |
+------+----------------+--------------+---------------------+--------------+-------+
6 rows in set (0.00 sec)
```


### 主从复制
#### Master配置
```sql
//192.168.0.106是slave从机的IP
GRANT REPLICATION SLAVE ON *.* to 'root'@'192.168.0.106' identified by 'Java@1234';
//192.168.0.107是slave从机的IP
GRANT REPLICATION SLAVE ON *.* to 'root'@'192.168.0.107' identified by 'Java@1234';
//刷新系统权限表的配置
FLUSH PRIVILEGES;
```

#### Slave配置
Slave配置相对简单一点。和Master一样，找到/etc/my.cnf配置文件，增加以下配置：
```sql
# 不要和其他mysql服务id重复即可
server-id=106
```

进入到mysql后，再输入以下命令：
```sql
CHANGE MASTER TO 
MASTER_HOST='imsweb.club',//主机IP
MASTER_USER='repl',//之前创建的用户账号
MASTER_PASSWORD='123456',//之前创建的用户密码
MASTER_LOG_FILE='mysql-bin.000003',//master主机的binlog日志名称
MASTER_LOG_POS=0,//binlog日志偏移量
master_port=3306;//端口
```

还没完，设置完之后需要启动：

```sql
# 启动slave服务
start slave;
```

启动完之后怎么校验是否启动成功呢？使用以下命令：
```sql
show slave status\G;
```

### 日志文件

#### 错误日志
对mysql的启动，运行，关闭进行记录。mysql DBA在遇到问题时候，应该查看该文件定位问题。
查看错误日志的文件名的命令是
```sql
mysql> show variables like 'log_error'\G;
*************************** 1. row ***************************
Variable_name: log_error
        Value: ./MACdeMacBook.local.err
1 row in set (0.00 sec)
```

#### 二进制日志(binlog)
作用:恢复(point-in-time)，复制（主从mysql），审计（安全）

执行对数据库更改的操作，不包括SELECT,SHOW这类。其它操作，就算没有导致数据发生变化也可能写入二进制日志。

二进制日志的路径是数据库所在目录（datadir);
```sql
mysql> show variables like 'datadir'\G;
*************************** 1. row ***************************
Variable_name: datadir
        Value: /usr/local/var/mysql/
1 row in set (0.00 sec)

#在mysql中使用shell查看
mysql> system ls /usr/local/var/mysql/;

```

```sql
mysql> show master status\G;
*************************** 1. row ***************************
             File: binlog.001014
         Position: 491
     Binlog_Do_DB:
 Binlog_Ignore_DB:
Executed_Gtid_Set:
1 row in set (0.00 sec)

ERROR:
No query specified

mysql> show binlog events in 'binlog.001014'\G;

```

#### 慢查询日志
帮助DBA定位可能存在问题的SQL语句，从而进行语句层面的优化.
运行时间超过阈值的语句记录，阈值通过long_query_time来设置，默认是10s。
```sql
mysql> show variables like 'long_query_time'\G;
mysql> show variables like 'long_slow_queries'\G;

```

#### 查询日志
记录所有对数据库请求的信息

#### 先写日志(redo log)
redo log日志也叫做WAL技术（Write- Ahead Logging），他是一种先写日志，并更新内存，最后再更新磁盘的技术，为了就是减少sql执行期间的数据库io操作，并且更新磁盘往往是在Mysql比较闲的时候，这样就大大减轻了Mysql的压力。

redo log是固定大小，是物理日志，属于InnoDB引擎的，并且写redo log是环状写日志的形式：redo log日志也叫做WAL技术（Write- Ahead Logging），他是一种先写日志，并更新内存，最后再更新磁盘的技术，为了就是减少sql执行期间的数据库io操作，并且更新磁盘往往是在Mysql比较闲的时候，这样就大大减轻了Mysql的压力。

redo log是固定大小，是物理日志，属于InnoDB引擎的，并且写redo log是环状写日志的形式：
如上图所示：若是四组的redo log文件，一组为1G的大小，那么四组就是4G的大小，其中write pos是记录当前的位置，有数据写入当前位置，那么write pos就会边写入边往后移。

check point记录擦除的位置，因为redo log是固定大小，所以当redo log满的时候，也就是write pos追上check point的时候，需要清除redo log的部分数据，清除的数据会被持久化到磁盘中，然后将check point向前移动。

redo log日志实现了即使在数据库出现异常宕机的时候，重启后之前的记录也不会丢失，这就是crash-safe能力。

binlog称为归档日志，是逻辑上的日志，它属于Mysql的Server层面的日志，记录着sql的原始逻辑，主要有两种模式：一个是statement格式记录的是原始的sql，而row格式则是记录行内容。

redo log和binlog记录的形式、内容不同，这两者日志都能通过自己记录的内容恢复数据。

之所以这两个日志同时存在，是因为刚开始Mysql自带的引擎MyISAM就没有crash-safe功能的，并且在此之前Mysql还没有InnoDB引擎，Mysql自带的binlog日志只是用来归档日志的，所以InnoDB引擎也就通过自己redo log日志来实现crash-safe功能。

###怎么查看索引是否生效？什么情况下索引会失效呢？
查看索引是否起作用可以使用explain关键字，查询后的语句中的key字段，若是使用了索引，该字段会展示索引的名字。
```sql
mysql> explain select * from test where id =1\G;
*************************** 1. row ***************************
           id: 1
  select_type: SIMPLE
        table: test
   partitions: NULL
         type: const
possible_keys: PRIMARY
          key: PRIMARY
      key_len: 4
          ref: const
         rows: 1
     filtered: 100.00
        Extra: NULL
1 row in set, 1 warning (0.00 sec)
```
#### explain列的解释
```
table：查询的数据表

type：这是重要的列，显示连接使用了何种类型。从最好到最差的连接类型为const、eq_reg、ref、range、index和all。

possible_keys：显示可能应用在这张表中的索引。如果为空，没有可能的索引。可以为相关的域从where语句中选择一个合适的语句

key： 实际使用的索引。如果为null，则没有使用索引。很少的情况下，mysql会选择优化不足的索引。这种情况下，可以在select语句中使用use index（indexname）来强制使用一个索引或者用ignore index（indexname）来强制mysql忽略索引

key_len：使用的索引的长度。在不损失精确性的情况下，长度越短越好

ref：显示索引的哪一列被使用了，如果可能的话，是一个常数

rows：mysql认为必须检查的用来返回请求数据的行数，mysql完成此请求扫描的行数。

extra：关于mysql如何解析查询的额外信息。将在表4.3中讨论，但这里可以看到的坏的例子是using temporary和using filesort，意思mysql根本不能使用索引，结果是检索会很慢。
```


