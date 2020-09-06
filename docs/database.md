[toc]

# mysql

## 数据库的基本操作

### 修改用户密码
`update user set authentication_string=password('123456') where user='ims';`

### 创建数据库

`CREATE DATABASE 数据库名;`

### 创建表
```
CREATE TABLE runoob_tbl(
    runoob_id INT NOT NULL AUTO_INCREMENT,
    runoob_title VARCHAR(100) NOT NULL,
    runoob_author VARCHAR(40) NOT NULL,
    submission_date DATE,
    PRIMARY KEY ( runoob_id )
    )ENGINE=InnoDB DEFAULT CHARSET=utf8;
```
InnoDB 主键使用的是聚簇索引，MyISAM 不管是主键索引，还是二级索引使用的都是非聚簇索引

### 删除表
`DROP TABLE table_name ;`

### 插入数据

```
INSERT INTO table_name ( field1, field2,...fieldN ) VALUES ( value1, value2,...valueN );
```

### 删除数据
```
DELETE FROM table_name [WHERE Clause]
DELETE FROM runoob_tbl WHERE runoob_id=3;
```

### 更新数据

```
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

## [MySQL 索引](https://www.cnblogs.com/wangsen/p/10864136.html)

MySQL索引的建立对于MySQL的高效运行是很重要的，索引可以大大提高MySQL的检索速度。

创建索引时，你需要确保该**索引是应用在 SQL 查询语句的条件**(一般作为 WHERE 子句的条件)。

上面都在说使用索引的好处，但过多的使用索引将会造成滥用。因此索引也会有它的缺点：虽然索引大大提高了查询速度，同时却会降低更新表的速度，如对表进行INSERT、UPDATE和DELETE。因为更新表时，MySQL不仅要保存数据，还要保存一下索引文件。

### 怎样提高查询性能

简单的理解：一张数据量比较大的表格如果没有添加任何索引，那我们在执行查询的时候
就会是进行全表扫描，逐行比对，这样的读取效率肯定很低，如果我们为数据创建了索引
索引的实现方式又是支持快速查询的这样我们只需要先查询索引中符合条件的，
然后再通过索引指向的数据行位置就可以实现快速定位数据了，不用全表扫描了。

### 索引存储位置

数据库索引是存储在磁盘上，当表中的数据量比较大时，索引的大小也跟着增长，达到几个G甚至更多。
当我们利用索引进行查询的时候，不可能把索引全部加载到内存中，只能加载一部分其他的都要从磁盘中读取后加载到内存。

### 索引分类

+ 主键索引
一个列为设置为主键会默认创建一个主键索引。
主键索引是唯一索引的特定类型。该索引要求主键中的每个值都唯一。当创建或更改表时可通过定义 `PRIMARY KEY` 约束来创建主键。一个表只能有一个主键列，且该列不能接受空值。 由于 PRIMARY KEY 约束确保唯一数据，所以经常用来定义标识列.

+ 唯一索引
严格来说主键索引是唯一索引的一种，这两个的区别是：主键索引不允许为null，唯一索引可以为null;
+ 单列索引
即一个索引只包含单个列，一个表可以有多个单列索引，但这不是组合索引。
+ 复合索引
即一个索引包含多个列。

### 为何Mysql选择B+树

#### 哈希表的两个缺点：
1. 哈希冲突
2. 不支持范围查找

#### 完全平衡二叉树

如果一个树的高度很大，如果查询的数据刚好在叶子节点那经历的磁盘Io的次数就是这个数的高度。
所以极端情况下平衡二叉树也不是优选。

#### B-Tree

先说说几个概念：

+ 度（节点的数据存储个数）也就是说B-Tree上的一个节点可以存储多个数据。
+ 叶节点具有相同的深度
+ 叶节点的指针为空
+ 节点中的数据从左到右递增排列
![avatar](dataStruct_pic/bTree.png)
这的确解决了树的高度问题，因为：B-Tree的节点可以存储多个值，高度肯定小于平衡二叉树，磁盘io的次数也会少。
但是在范围查找方面较比B+Tree差点。
题外话：B-Tree和BTree是一种树。

#### B+ Tree
B+树的表示要比B树要“胖”，原因在于B+树中的非叶子节点会冗余一份在叶子节点中，并且叶子节点之间用指针相连。

![avatar](dataStruct_pic/bPlusTree.png)
mysql的实现中，B+Tree是把非叶子节点中只存储索引，不存储数据，只有叶子节点存储数据，
这样节点中的空间更多的存储了索引，增加了度。最大程度的降低了树的高度

加上一个节点的大小设置成为一页或页的倍数，一次磁盘io就可以读出了一个节点中的很多数据，
加载到内存中再进行查找就很快了

所以最后总结：
使用B+Tree:可以提高查询索引时的磁盘IO效率，并且可以提高范围查询的效率，并且B+树里的元素也是有序的。

### MyISAM和InnoDB的B+Tree实现

索引是数据库引擎去实现的，在建立表的时候都会指定，数据库引擎是一种插拔式的，根据自己的选择去决定使用那个

#### MyISAM索引实现（非聚集）

MYISAM中叶子节点的数据区域存储的是**数据记录的地址**。
MyISAM中的主键索引和辅助引是没有区别的，其叶子节点存放的都是数据记录的地址。


![avatar](dataStruct_pic/myISAM.png)

#### InnoDB的索引实现（聚集）

InnoDB中的叶子节点数据区域存储的内容和主键索引和辅助索引是有区别的：

主键索引存储的就是索引+数据（index+data）

![avatar](dataStruct_pic/innoDB.png)
Innodb的主键索引要比MyISAM的主键索引查询效率要高，因为找到主键索引就找到了数据,MyISAM还有通过地址查询一次。

辅助索引存储的是主键的值

![avatar](dataStruct_pic/innoDB2.png)
因此可以看出InnoDB的辅助索引会发生两次，一次通过辅助索引查询主键索引，一次是通过主键索引查询到数据。


###聚集索引和非聚集索引

#### 聚集索引

数据行的物理顺序与列值（一般是主键的那一列）的逻辑顺序相同，一个表中只能拥有一个聚集索引。
聚集索引的叶子节点存放有对应的数据节点，可以直接获取到对应的数据，
如果不创建索引，系统会自动创建一个隐含列作为表的聚集索引。
最好还是在创建表的时候添加聚集索引
在经常用于查询或聚合条件的字段上建立聚集索引。这类查询条件包括 between, >, <,group by, max,min, count等。

#### 非聚集索引

**叶子节点存放的不是实际数据，而是指向实际数据的指针。**
数据行的物理顺序与列值的逻辑顺序不相同，一个表中可以拥有多个非聚集索引。
聚集索引以外的索引都是非聚集索引,细分可以分为：普通索引，唯一索引，全文索引

## [mysql常见的优化策略](https://www.cnblogs.com/wangsen/p/10871996.html)

[csdn](https://blog.csdn.net/long690276759/article/details/79571421)

1. 对查询进行优化，应**尽量避免全表扫描**，首先应考虑在 where 及 order by 涉及的列上建立**索引**。
2. 应尽量避免在 where 子句中对字段进行 null 值判断，否则将导致引擎放弃使用索引而进行全表扫描，如：select id from t where num is null可以在num上设置默认值0，确保表中num列没有null值，然后这样查询：select id from t where num=0
3. 应尽量避免在 where 子句中使用!=或<>操作符，否则引擎将放弃使用索引而进行全表扫描。
4. 应尽量避免在 where 子句中使用or 来连接条件，否则将导致引擎放弃使用索引而进行全表扫描，如：select id from t where num=10 or num=20可以这样查询：select id from t where num=10 union all select id from t where num=20
5. in 和 not in 也要慎用，否则会导致全表扫描，如：select id from t where num in(1,2,3) 对于连续的数值，能用 between 就不要用 in 了：select id from t where num between 1 and 3
6. 下面的查询也将导致全表扫描：select id from t where name like ‘%李%’若要提高效率，可以考虑全文检索。
7. 如果在 where 子句中使用参数，也会导致全表扫描。因为SQL只有在运行时才会解析局部变量，但优化程序不能将访问计划的选择推迟到运行时；它必须在编译时进行选择。然 而，如果在编译时建立访问计划，变量的值还是未知的，因而无法作为索引选择的输入项。如下面语句将进行全表扫描：select id from t where num=@num可以改为强制查询使用索引：select id from t with(index(索引名)) where num=@num
8. 应尽量避免在 where 子句中对字段进行表达式操作，这将导致引擎放弃使用索引而进行全表扫描。如：select id from t where num/2=100应改为:select id from t where num=100*2
9. 应尽量避免在where子句中对字段进行函数操作，这将导致引擎放弃使用索引而进行全表扫描。如：select id from t where substring(name,1,3)=’abc’ ，name以abc开头的id应改为:
select id from t where name like ‘abc%’
10. 不要在 where 子句中的“=”左边进行函数、算术运算或其他表达式运算，否则系统将可能无法正确使用索引。

## 数据库的理论

### 关系型数据库和非关系数据库的特点，

简单来说，关系模型指的就是二维表格模型，而一个关系型数据库就是由二维表及其之间的联系所组成的一个数据组织。

非关系型数据库提出另一种理念，例如，以键值对存储，且结构不固定，每一个元组可以有不一样的字段，每个元组可以根据需要增加一些自己的键值对，这 样就不会局限于固定的结构，可以减少一些时间和空间的开销。使用这种方式，用户可以根据需要去添加自己需要的字段，这样，为了获取用户的不同信息，不需要 像关系型数据库中，要对多表进行关联查询。仅需要根据id取出相应的value就可以完成查询。但非关系型数据库由于很少的约束，他也不能够提供像SQL 所提供的where这种对于字段属性值情况的查询。并且难以体现设计的完整性。他只适合存储一些较为简单的数据，对于需要进行较复杂查询的数据，SQL数 据库显的更为合适。

关系型数据库的最大特点就是事务的一致性：传统的关系型数据库读写操作都是事务的，具有ACID的特点，这个特性使得关系型数据库可以用于几乎所有对一致性有要求的系统中，如典型的银行系统。

但是，在网页应用中，尤其是SNS应用中，一致性却不是显得那么重要，用户A看到的内容和用户B看到同一用户C内容更新不一致是可以容忍的，或者 说，两个人看到同一好友的数据更新的时间差那么几秒是可以容忍的，因此，关系型数据库的最大特点在这里已经无用武之地，起码不是那么重要了。

相反地，关系型数据库为了维护一致性所付出的巨大代价就是其读写性能比较差，而像微博、facebook这类SNS的应用，对并发读写能力要求极 高，关系型数据库已经无法应付(在读方面，传统上为了克服关系型数据库缺陷，提高性能，都是增加一级memcache来静态化网页，而在SNS中，变化太 快，memchache已经无能为力了)，因此，必须用新的一种数据结构存储来代替关系数据库。

关系数据库的另一个特点就是其具有固定的表结构，因此，其扩展性极差，而在SNS中，系统的升级，功能的增加，往往意味着数据结构巨大变动，这一点关系型数据库也难以应付，需要新的结构化数据存储。

于是，非关系型数据库应运而生，由于不可能用一种数据结构化存储应付所有的新的需求，因此，非关系型数据库严格上不是一种数据库，应该是一种数据结构化存储方法的集合。

必须强调的是，数据的持久存储，尤其是海量数据的持久存储，还是需要一种关系数据库这员老将。


### 非关系型数据库分类：
主要分为以下几类：

1. 面向高性能并发读写的key-value数据库：

    key-value数据库的主要特点即使具有极高的并发读写性能，Redis,Tokyo Cabinet,Flare就是这类的代表

2. 面向海量数据访问的面向文档数据库：

    这类数据库的特点是，可以在海量的数据中快速的查询数据，典型代表为MongoDB以及CouchDB

3. 面向可扩展性的分布式数据库：

    这类数据库想解决的问题就是传统数据库存在可扩展性上的缺陷，这类数据库可以适应数据量的增加以及数据结构的变化

[关系型数据库和非关系型数据库](http://my.oschina.net/u/1773689/blog/364548)


## [最全MySQL面试题和答案](https://www.cnblogs.com/lijiasnong/p/9963905.html)
