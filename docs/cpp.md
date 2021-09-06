#

## [c++常见问题汇总](https://www.cnblogs.com/inception6-lxc/p/8686156.html)

## C++关键字解释
###  mutable

mutable的中文意思是“可变的，易变的”，跟constant（既C++中的const）是反义词。

<font color="#0099ff">在C++中，mutable也是为了突破const的限制而设置的</font>。   
被mutable修饰的变量，将永远处于可变的状态，即使在一个const函数中。   
我们知道，如果类的成员函数不会改变对象的状态，那么这个成员函数一般会声明成const的。    
但是，有些时候，我们需要在const的函数里面修改一些跟类状态无关的数据成员，那么这个数据成员就应该被mutalbe来修饰。

### final
禁用继承 & 禁用重写     
C++11中允许将类标记为final，方法时直接在类名称后面使用关键字final，如此，意味着继承该类会导致编译错误。
C++中还允许将方法标记为fianal，这意味着无法再子类中重写该方法。这时final关键字至于方法参数列表后面，
实例如下：
```cpp linenums="1" hl_lines="1 4"
class Super final
{
  public:
    virtual void SomeMethod() final;
  //......
};
```
### explicit

首先, C++中的<font color="#0099ff">explicit关键字只能用于修饰只有一个参数</font>的类构造函数,   
或者<font color="#0099ff">除了第一个参数以外的其他参数都有默认值的时候, </font>
由于无参数的构造函数和多参数的构造函数总是显示调用，这种情况在构造函数前加explicit无意义。  
它的作用是表明该构造函数是显示的, 而非隐式的, 跟它相对应的另一个关键字是implicit,  
类构造函数默认情况下即声明为implicit, explicit关键字的作用就是防止类构造函数的隐式自动转换.

## [this指针&成员函数](https://blog.csdn.net/xuruhua/article/details/80656580)
类类型`* const this`指针并不是对象本身的一部分，不影响sizeof的结果 this的作用域在类”成员函数”的内部 this指针是”类成员函数”的第一个默认隐含参数，编译器自动维护传递，类编写者不能显式传递 只有在类的非静态成员函数中才可以使用this指针，其它任何函数都不可以.

1. 类成员函数与非成员函数都是带有类命名空间的函数.
2. 通过对象或者对象指针调用非static成员函数的时候,会把对象的地址传递给非静态成员函数this变量,且不判断this是否为空,所以普通函数指针无法接收非static成员函数指针.
3. 普通函数指针可以接收static成员函数的指针,并绕过类的命名空间调用.

```cpp  linenums="1" hl_lines="5 23 24" 
class Test {
public:
  void FunTest() {
    cout << "this:" << this << endl; // can be call when pt == NULL
    //_data = 10;//segmentation fault when pt ==NULL
  }
  static void func() { cout << "static func" << endl; }
  int _data;
};

int main() {
  Test t;
  t.FunTest();   // lea -0x14(%rbp),%rax; mov %rax,%rdi; callq <Test::FunTest()>
  t.func();      // callq <Test::func()>;
  Test *pt = &t; // lea -0x14(%rbp),%rax;mov  %rax,-0x10(%rbp)
  pt->FunTest(); // //lea -0x14(%rbp),%rax; mov %rax,%rdi; callq
                 // <Test::FunTest()>
  pt = NULL;     // movq   $0x0,-0x10(%rbp)
  pt->FunTest(); // mov  -0x10(%rbp),%rax; mov  %rax,%rdi; callq
                 // <Test::FunTest()>
  /*error| Call to non-static member function without an object argument
   * [member_call_without_object]*/
  void (*funcPtr)() = Test::FunTest;//error
  auto funcPtr = std::bind(&Test::FunTest,&t);//ok
  void (*sfuncPtr)() = Test::func;//ok
  sfuncPtr();
  return 0;
}
```
## 继承

在C++中继承主要有三种关系：public、protected和private。这三种继承关系中public
继承是最为常用的一种继承关系，代表了接口继承含义，而他们分别具体代表了什么含义呢？
### 1. public
从语义角度上来说，public继承是一种接口继承，根据面向对象中的关系而言就是，子类
可以代替父类完成父类接口所声明的行为，也就是必须符合“Liskov替换原则（LSP）”，
此时子类可以自动转换成为父类的接口，完成接口转换。
从语法角度上来说，public继承会保留父类中成员（包括函数和变量等）的可见性不变，
也就是说，如果父类中的某个函数是public的，那么在被子类继承后仍然是public的。

### 2. protected
从语义角度上来说，protected继承是一种实现继承，根据面向对象中的关系而言就是，
子类不能代替父类完成父类接口所声明的行为，也就是不符合“Liskov替换原则（LSP）”，
此时子类不能自动转换成为父类的接口，就算通过类型转换（static_cast和dynamic_cast）
也会得到一个空指针。
从语法角度上来说，protected继承会将父类中的public可见性的成员修改成为protected
可见性，相当于在子类中引入了protected成员，这样一来在子类中同样还是可以调用父
类的protected和public成员，子类的子类就也可以调用被protected继承的父类的protected
和public成员。
例如：
```cpp
class CSample1 {
protected:
  void printProtected() {}
public:
  void printPublic() {}
};
class CSample2 : protected CSample1 {};
class CSample3 : public CSample2 {
  void print3() {
    printProtected();
    printPublic();
  }
};
```

### 3. private
从语义角度上来说，private继承是一种实现继承，根据面向对象中的关系而言就是，
子类不能代替父类完成父类接口所声明的行为，也就是不符合“Liskov替换原则（LSP）”，
此时子类不能自动转换成为父类的接口，就算通过类型转换（static_cast和dynamic_cast）
也会得到一个空指针。
从语法角度上来说，private继承会将父类中的public和protected可见性的成员修改成为
private可见性，这样一来虽然子类中同样还是可以调用父类的protected和public成员，
但是在子类的子类就不可以再调用被private继承的父类的成员了(private成员不能被继承)。


在面向对象的理论中有两种概念：接口、实现，所以就出现了所谓的接口继承和实现继
承两种关系。而protected和private就是实现继承中所要用到的，其实protected和private
两者则约束继承时并没有形成两种不同的继承类别，而仅仅只是为了方便C++类方法的传递
调用而设计的，其实在java这样面向对象要求更为严格的语言当中，没有实现继承，他必须
通过委托方式来完成这一概念，如果熟悉java就会明白，如果一个对象要使用另外一个对象
的接口功能，而自身又不能够充当该对象所扮演的角色时，就会通过委托来完成，这样一来
就必须在对象中包含一个委托对象，通过对象调用语法来完成功能；在C++中就可以通过
protected和private继承来完成java中的委托关系（当然C++也可以形成对象委托关系），
那么这种情况下protected继承就容许委托可以传递（也就是被多级子类调用），而private
继承是不容许委托被传递的。

## 容器

STL各类型容器实现，STL共有六大组件
+ STL提供六大组件，彼此可以组合套用：

1.  容器（Containers）：各种数据结构，如：序列式容器vector、list、deque、关联式容器set、map、multiset、multimap。用来存放数据。从实现的角度来看，STL容器是一种class template。

2.  算法（algorithms）：各种常用算法，如：sort、search、copy、erase。从实现的角度来看，STL算法是一种 function template。注意一个问题：任何的一个STL算法，都需要获得由一对迭代器所标示的区间，用来表示操作范围。这一对迭代器所标示的区间都是前闭后开区间，例如[first, last)

3.  迭代器（iterators）：容器与算法之间的胶合剂，是所谓的“泛型指针”。共有五种类型，以及其他衍生变化。从实现的角度来看，迭代器是一种将 operator*、operator->、operator++、operator- - 等指针相关操作进行重载的class template。所有STL容器都有自己专属的迭代器，只有容器本身才知道如何遍历自己的元素。原生指针(native pointer)也是一种迭代器。

4.  仿函数（functors）：行为类似函数，可作为算法的某种策略（policy）。从实现的角度来看，仿函数是一种重载了operator（）的class或class template。一般的函数指针也可视为狭义的仿函数。

5.  配接器（adapters）：一种用来修饰容器、仿函数、迭代器接口的东西。例如：STL提供的queue 和 stack，虽然看似容器，但其实只能算是一种容器配接器，因为它们的底部完全借助deque，所有操作都由底层的deque供应。改变 functors接口者，称为function adapter；改变 container 接口者，称为container adapter；改变iterator接口者，称为iterator adapter。

6.  配置器（allocators）：负责空间配置与管理。从实现的角度来看，配置器是一个实现了动态空间配置、空间管理、空间释放的class template。

这六大组件的交互关系：container（容器） 通过 allocator（配置器） 取得数据储存空间，algorithm（算法）通过 iterator（迭代器）存取 container（容器） 内容，functor（仿函数） 可以协助 algorithm（算法） 完成不同的策略变化，adapter（配接器） 可以修饰或套接 functor（仿函数）

* 序列式容器：
    vector-数组，元素不够时再重新分配内存，拷贝原来数组的元素到新分配的数组中。
    list－单链表。
    deque-分配中央控制器map(并非map容器)，map记录着一系列的固定长度的数组的地址. 记住这个map仅仅保存的是数组的地址,真正的数据在数组中存放着. deque先从map中央的位置(因为双向队列，前后都可以插入元素)找到一个数组地址，向该数组中放入数据，数组不够时继续在map中找空闲的数组来存数据。当map也不够时重新分配内存当作新的map,把原来map中的内容copy的新map中。所以使用deque的复杂度要大于vector，尽量使用vector。

stack-基于deque。
    queue-基于deque。
    heap-完全二叉树，使用最大堆排序，以数组(vector)的形式存放。
    priority_queue-基于heap。
    slist-双向链表。

* 关联式容器：
    1. set,map,multiset,multimap-基于红黑树(RB-tree)，
    一种加上了额外平衡条件的二叉搜索树。
    2. hash_map,hash_set,hash_multiset,hash_multimap-基于hashtable。
    hash table-散列表:
       将待存数据的key经过映射函数变成一个数组(一般是vector)的索引，例如：数据的key%数组的大小＝数组的索引(一般文本通过算法也可以转换为数字)，然后将数据当作此索引的数组元素。有些数据的key经过算法的转换可能是同一个数组的索引值(碰撞问题，可以用线性探测，二次探测来解决)，STL是用开链的方法来解决的，每一个数组的元素维护一个list，他把相同索引值的数据存入一个list，这样当list比较短时执行**删除，插入，搜索**等算法比较快。

### 容器细分类
**vector、string、deque和list** 被称为标准序列容器，

**set、multiset、map和multimap** 是标准关联容器。

非标准序列容器slist和rope。slist是一个单向链表，rope本质上是一个重型字符串。

非标准关联容器 hash_set、hash_multiset、hash_map和hash_multimap。

标准非STL容器是指"可以认为它们是容器，但是他们并不满足STL容器的所有要求"

标准非STL容器包括: **数组、bitset、valarray、stack、queue和priority_queue** 。
### map与set的差异

map和set都是C++的关联容器，其底层实现都是红黑树（RB-Tree）。由于 map 和set所开放的各种操作接口，RB-tree 也都提供了，所以几乎所有的 map 和set的操作行为，都只是转调 RB-tree 的操作行为。
map和set区别在于：

（1）map中的元素是key-value（关键字—值）对：关键字起到索引的作用，值则表示与索引相关联的数据；Set与之相对就是关键字的简单集合，set中每个元素只包含一个关键字。

（2）set的迭代器是const的，不允许修改元素的值；map允许修改value，但不允许修改key。其原因是因为map和set是根据关键字排序来保证其有序性的，如果允许修改key的话，那么首先需要删除该键，然后调节平衡，再插入修改后的键值，调节平衡，如此一来，严重破坏了map和set的结构，导致iterator失效，不知道应该指向改变前的位置，还是指向改变后的位置。所以STL中将set的迭代器设置成const，不允许修改迭代器的值；而map的迭代器则不允许修改key值，允许修改value值。

（3）map支持下标操作，set不支持下标操作。map可以用key做下标，map的下标运算符[ ]将关键码作为下标去执行查找，如果关键码不存在，则插入一个具有该关键码和mapped_type类型默认值的元素至map中，因此下标运算符[ ]在map应用中需要慎用，const_map不能用，只希望确定某一个关键值是否存在而不希望插入元素时也不应该使用，mapped_type类型没有默认值也不应该使用。如果find能解决需要，尽可能用find。

### list和vector有什么区别？

+ vector拥有一段连续的内存空间，因此支持随机存取，如果需要高效的随即存取，而不在乎插入和删除的效率，使用vector。
+ list拥有一段不连续的内存空间，因此不支持随机存取，如果需要大量的插入和删除，而不关心随即存取，则应使用list。

## c++11新特性
### lambda
必须包含捕获列表和函数体，可以忽略参数列表，返回类型'
```c
auto f= []{return 1;};
```
捕获列表用法：
```c
[],
[names]
[=]
[&]
[&,identifier_list] //identifier_list 采用值捕获
[=,identifier_list] //identifier_list 采用引用捕获
```
### 智能指针实现

#### share_ptr

* 一个友元类：weak_ptr
* 两个成员变量:
    1. 原始指针 `T*ptr`
    2. 类静态成员`map<T *, int>` 记录原始指针的计数

#### weak_ptr 
一个成员变量: `shared_ptr<T> *_ptr`

使用强指针初始化，使用lock函数获取强指针


```cpp
#include <iostream>
#include <map>
using namespace std;

template <typename T> class my_weak_ptr;

template <typename T> class my_shared_ptr {
public:
  my_shared_ptr(T *ptr = NULL);                  //构造方法
  ~my_shared_ptr();                              //析构方法
  my_shared_ptr(my_shared_ptr<T> &src);            //拷贝构造
  my_shared_ptr &operator=(my_shared_ptr<T> &src); //赋值运算符重载
  T &operator*();                              //解引用运算符重载
  T *operator->();                             //成员运算符重载
private:
  T *_ptr;
  static map<T *, int> _map; //对象指针作为key，value是计数变量
  friend class my_weak_ptr<T>;//weak_ptr 作为友元
};

template <typename T> map<T *, int> my_shared_ptr<T>::_map;

template <typename T>
my_shared_ptr<T>::my_shared_ptr(T *ptr) //构造方法
{
  cout << "my_shared_ptr的构造方法正被调用！" << endl;
  _ptr = ptr;
  _map.insert(make_pair(_ptr, 1));
}

template <typename T>
my_shared_ptr<T>::~my_shared_ptr() //析构方法
{
  cout << "my_shared_ptr的析构方法正被调用！" << endl;
  if (--_map[_ptr] <= 0 && NULL != _ptr) {
    delete _ptr;
    _ptr = NULL;
    _map.erase(_ptr);
  }
}

template <typename T>
my_shared_ptr<T>::my_shared_ptr(my_shared_ptr<T> &src) //拷贝构造
{
  cout << "my_shared_ptr的拷贝构造方法正被调用！" << endl;
  _ptr = src._ptr;
  _map[_ptr]++;
}

template <typename T>
my_shared_ptr<T> &my_shared_ptr<T>::operator=(my_shared_ptr<T> &src) //赋值运算符重载
{
  if (_ptr == src._ptr) {
    return *this;
  }

  if (--_map[_ptr] <= 0 && NULL != _ptr) {
    delete _ptr;
    _ptr = NULL;
    _map.erase(_ptr);
  }

  _ptr = src._ptr;
  _map[_ptr]++;
  return *this;
}

template <typename T>
T &my_shared_ptr<T>::operator*() //解引用运算符重载
{
  return *_ptr;
}

template <typename T>
T *my_shared_ptr<T>::operator->() //成员运算符重载
{
  return _ptr;
}

/******* weak_ptr define *******************/
template <typename T> class my_weak_ptr {
public:
  my_weak_ptr() {}                  //默认构造方法
  my_weak_ptr(my_shared_ptr<T> &src); //构造方法
  my_shared_ptr<T> lock();

private:
  my_shared_ptr<T> *_ptr;
};

template <typename T>
my_weak_ptr<T>::my_weak_ptr(my_shared_ptr<T> &src) //使用shared_ptr构造
{
  _ptr = &src;
}

template <typename T>
my_shared_ptr<T> my_weak_ptr<T>::lock() //返回一个shared_ptr
{
  int count = _ptr->_map[_ptr->_ptr];
  if (count > 0) //从shared_ptr获得一个可用的shared_ptr对象。从而操作资源
  {
    return *_ptr;//接收方copy一份，增加了计数
  } else {
    return NULL;
  }
}
//测试类
class B;
class A {
public:
  my_weak_ptr<B> _ptr_B; //类内存放的是弱指针，将在合适的时间转变成强指针！
};
class B {
public:
  my_weak_ptr<A> _ptr_A;
};

int main() {
  my_shared_ptr<A> ptr_A(new A);
  my_shared_ptr<B> ptr_B(new B);
  ptr_A->_ptr_B = ptr_B;
  ptr_B->_ptr_A = ptr_A;
  return 0;
}
```
#### auto_ptr不安全的原因

下面例子反应auto_ptr两个问题

1. 不安全，赋值以及copy构造会把参数置空
2. 不满足stl <font color="#0099ff">常引用copy构造(const X&)</font>
```cpp  linenums="1" hl_lines="21"
#include <iostream>
#include <list>
#include <memory>

using namespace std;

int main() {
  auto_ptr<string> films[3] = {
      auto_ptr<string>(new string("one")),
      auto_ptr<string>(new string("two")),
      auto_ptr<string>(new string("three")),
  };
  auto_ptr<string> pwin;
  pwin = films[1]; //films[1],引用为空
  cout << "pwin: " << *pwin << endl;
  for (auto &s : films)
    cout << *s << endl; //空指针访问，运行时错误

  std::list<std::auto_ptr<int>> la;
  std::auto_ptr<int> p1(new int(1));
  la.push_back(p1); // compiling-error!
  /* can't convert 'from ‘const std::auto_ptr<int>’ to ‘std::auto_ptr<int>::element_type*/
  return 0;
}
```


如果使用unique_ptr替换auto_ptr,编译错误
```cpp
#include <iostream>
#include <string>
#include <memory>

using namespace std;

int main()
{
    unique_ptr<string> films[3] =
    {
        unique_ptr<string> (new string("one")),
        unique_ptr<string> (new string("two")),
        unique_ptr<string> (new string("three")),
    };
    unique_ptr<string> pwin;
    pwin = films[1]; // films[1] lose ownership

    cout << "films data is: " << endl;
    for(unique_ptr<string> &s : films)
        cout << *s << endl;
    cout << "pwin: " << *pwin << endl;
    return 0;
}
```

### C++智能指针的正确使用方式

#### unique_ptr：专属所有权
我们大多数场景下用到的应该都是unique_ptr。

unique_ptr代表的是专属所有权，即由unique_ptr管理的内存，只能被一个对象持有。
```cpp
//如果想要把w复制给w2, 是不可以的。因为复制从语义上来说，两个对象将共享同一块内存。
//unique_ptr不支持复制和赋值，如下：

auto w = std::make_unique<Widget>();
auto w2 = w; // 编译错误

//因此，unique_ptr只支持移动, 即如下：

auto w = std::make_unique<Widget>();
auto w2 = std::move(w); // w2获得内存所有权，w此时等于nullptr

```
**性能**

因为C++的zero cost abstraction的特点，unique_ptr在默认情况下和裸指针的大小是一样的。

所以内存上没有任何的额外消耗，性能是最优的。
使用场景1：忘记delete
使用场景2：异常安全
```
void process()
{
    Widget* w = new Widget();
    w->do_something(); // 可能会发生异常
    delete w;
}
```

#### shared_ptr：共享所有权
在使用shared_ptr之前应该考虑，是否真的需要使用shared_ptr, 而非unique_ptr。  
shared_ptr代表的是共享所有权，即多个shared_ptr可以共享同一块内存。   
因此，从语义上来看，shared_ptr是支持复制的

**性能**

1. 内存占用高
shared_ptr的内存占用是裸指针的两倍。因为除了要管理一个裸指针外，还要维护一个引用计数。
因此相比于unique_ptr, shared_ptr的内存占用更高
2. 原子操作性能低
考虑到线程安全问题，引用计数的增减必须是原子操作。而原子操作一般情况下都比非原子操作慢。
3. 使用移动优化性能
shared_ptr在性能上固然是低于unique_ptr。而通常情况，我们也可以尽量避免shared_ptr复制。
如果，一个shared_ptr需要将所有权共享给另外一个新的shared_ptr，而我们确定在之后的代码中都不再使用这个shared_ptr，那么这是一个非常鲜明的移动语义。
对于此种场景，我们尽量使用std::move，将shared_ptr转移给新的对象。因为移动不用增加引用计数，因此性能比复制更好。

使用场景

1. shared_ptr通常使用在共享权不明的场景。有可能多个对象同时管理同一个内存时。
2. 对象的延迟销毁。陈硕在《Linux多线程服务器端编程》中提到，当一个对象的析构非常耗时，甚至影响到了关键线程的速度。可以使用BlockingQueue<std::shared_ptr<void>>将对象转移到另外一个线程中释放，从而解放关键线程。

#### weak_ptr
weak_ptr是为了解决shared_ptr双向引用的问题。

### [bind](https://www.jianshu.com/p/af303820fe0f)
#### bind作用
bind作用于函数上（包括普通函数，类成员函数等），将函数改造为一个类的对象，这个类里面实现了operator()操作符，使得这个对象能像函数一样能使用()调用。这样有什么好处呢？

1. 函数对象操作方式和函数几乎一致，不会带来新的语法难度。
2. 函数对象可以预设函数的部分参数，更加灵活。
3. 函数对象可以绑定到类的成员函数。


## 对象实例化问题

### 设计一个不能被继承的类。
**构造函数或析构函数为private**，所以该类是无法被继承的，
构造函数为私有的类，其对象只能是static对象，单例模式中，应该把构造函数设置为私有，防止在外部创建对象。

### 定义一个只能在堆上定义对象的类
**析构函数定义为private**，在栈上不能自动调用析构函数，只能手动调用。也可以将构造函数定义为private，但这样需要手动写一个函数实现对象的构造。

### 定义一个只能在栈上定义对象的类
**operator new和operator delete定义为private**，这样使用new操作符创建对象时候，无法调用operator new，delete销毁对象也无法调用operator delete。

## 模板
### 类特化与偏特化
模板为什么要特化，因为编译器认为，对于特定的类型，如果你能对某一功能更好的实现，那么就该听你的。

模板分为类模板与函数模板，特化分为全特化与偏特化。

* 全特化就是限定死模板实现的具体类型，
* 偏特化就是如果这个模板有多个类型，那么只限定其中的一部分。
```cpp
#include <iostream>
using namespace std;
template <typename T1, typename T2>
class Test {
public:
  Test(T1 i, T2 j) : a(i), b(j) { cout << "模板类" << endl; }

private:
  T1 a;
  T2 b;
};
//全特
template <>
class Test<int, char> {
public:
  Test(int i, char j) : a(i), b(j) { cout << "全特化" << a << b << endl; }

private:
  int a;
  char b;
};
//偏特
template <typename T2>
class Test<char, T2> {
public:
  Test(char i, T2 j) : a(i), b(j) { cout << "偏特化" << endl; }

private:
  char a;
  T2 b;
};

int main() {
  Test<double, double> t1(0.1, 0.2);
  Test<int, char> t2(1, 'A');
  Test<char, bool> t3('A', true);
  return 0;
}
```

## [构造函数详解](https://www.cnblogs.com/alantu2018/p/8459250.html)

### C++三五法则
1. 什么是三法则

    C++三法则：如果需要析构函数，则一定需要拷贝构造函数和拷贝赋值操作符。

2. 什么是五法则

    在较新的 C++11 标准中，为了支持移动语义，又增加了移动构造函数和移动赋值运算符，这样共有五个特殊的成员函数，所以又称为“C++五法则”；
    也就是说，“三法则”是针对较旧的 C++89 标准说的，“五法则”是针对较新的 C++11 标准说的；为了统一称呼，后来人们干把它叫做“C++ 三/五法则”；

### 那些是拷贝构造函数
```
X::X(X);               //不是copy，导致递归调用
X::X(X&);              //拷贝构造函数
X::X(const X&);        //常拷贝构造函数
X::X(X&, int a=1);     //拷贝构造函数
X::X(X&, int a=1, int b=2);  //拷贝构造函数

class Base {
protected:
  void printProtected() {}

public:
  void printPublic() {}
  void printPublic() const {}
  Base() {}
  Base(Base &a) {
    cout << "copy" << endl;
    a.printPublic();
  }
  Base(const Base &a) {
    cout << "const copy" << endl;
    a.printPublic();
  }
};

int main() {
  const Base s;
  Base s1(s);
  return 0;
}

```

解答：对于一个类X, 如果一个构造函数的第一个参数是下列之一:

* X&
* const X&
* volatile X&
* const volatile X&

且没有其他参数或其他参数都有默认值,那么这个函数是拷贝构造函数.

### 拷贝构造调用时机
1. 对象作为入参
2. 对象作为返回值
3. 用一个对象初始化另一个对象

### 浅拷贝

 所谓浅拷贝，指的是在对象复制时，只对对象中的数据成员进行简单的赋值，默认拷贝构造函数执行的也是浅拷贝。大多情况下“浅拷贝”已经能很好地工作了，但是一旦对象存在了动态成员，那么浅拷贝就会出问题了

### 深拷贝

 在“深拷贝”的情况下，对于对象中动态成员，就不能仅仅简单地赋值了，而应该重新动态分配空间

### 防止默认拷贝发生

通过对对象复制的分析，我们发现对象的复制大多在进行“值传递”时发生，这里有一个小技巧可以防止按值传递——声明一个私有拷贝构造函数。甚至不必去定义这个拷贝构造函数，这样因为拷贝构造函数是私有的，如果用户试图按值传递或函数返回该类对象，将得到一个编译错误，从而可以避免按值传递或返回对象。



###  一个类中可以存在多于一个的拷贝构造函数吗?
解答：类中可以存在超过一个拷贝构造函数。
```
class X { 
public: 
  X(const X&); // const 的拷贝构造 
  X(X&); // 非const的拷贝构造 
};
```

注意,如果一个类中只存在一个参数为 X& 的拷贝构造函数,那么就不能使用const X或volatile X的对象实行拷贝初始化.
  如果一个类中没有定义拷贝构造函数,那么编译器会自动产生一个默认的拷贝构造函数。
  这个默认的参数可能为 X::X(const X&)或 X::X(X&),由编译器根据上下文决定选择哪一个。

### C++构造函数以及析构函数的若干面试问题
 
Q1：构造函数能否重载，析构函数能否重载，为什么？
A1：构造函数可以，析构函数不可以。

Q2：析构函数为什么一般情况下要声明为虚函数？
A2：虚函数是实现多态的基础，当我们通过基类的指针是析构子类对象时候，如果不定义成虚函数，那只调用基类的析构函数，子类的析构函数将不会被调用。如       果定义为虚函数，则子类父类的析构函数都会被调用。

Q3：什么情况下必须定义拷贝构造函数？
A3：当类的对象用于函数值传递时（值参数，返回类对象），拷贝构造函数会被调用。如果对象复制并非简单的值拷贝，那就必须定义拷贝构造函数。例如大的堆       栈数据拷贝。如果定义了拷贝构造函数，那也必须重载赋值操作符。

## 仿函数与函数指针

仿函数在编译期可以内联，速度可能比函数指针快。

## [四个强转](https://blog.csdn.net/Bob__yuan/article/details/88044361)

### 1）static_cast
  这应该四种中是最常见的。用法为 static_cast<type-id> (expression)。
该运算符把 expression 转换为 type-id 类型，但没有运行时类型检查来保证转换的安全性。
主要用法如下：

1. 用于类层次结构中基类（父类）和派生类（子类）之间指针或引用的转换。
   进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；
   进行下行转换（把基类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以是不安全的。
2. 用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
3. 把空指针转换成目标类型的空指针。
4. 把任何类型的表达式转换成void类型。

### 2）const_cast

上边的 static_cast 不能将 const int\* 转成 int\*，const_cast 就可以，用法为 const_cast<type-i> (expression)。如下面代码

### 3）reinterpret_cast

reinterpret_cast 主要有三种强制转换用途：

1. 改变指针或引用的类型
2. 将指针或引用转换为一个足够长度的整形
3. 将整型转换为指针或引用类型。

用法为 reinterpret_cast <type-id> (expression)。
type-id 必须是一个指针、引用、算术类型、函数针或者成员指针。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
  我们映射到的类型仅仅是为了故弄玄虚和其他目的，这是所有映射中最危险的。(这句话是C++编程思想中的原话)。因此, 你需要谨慎使用 reinterpret_cast。

### 4）dynamic_cast(运行时处理)

用法为 dynamic_cast<type-id> (expression)。
几个特点如下：

1. 其他三种都是编译时完成的，dynamic_cast 是 **运行时处理的** ，运行时要进行类型检查。
2. 不能用于内置的基本数据类型的强制转换
3. dynamic_cast 要求 <> 内所描述的<font color="#ff0000">目标类型必须为指针或引用</font>。dynamic_cast 如果成功返回的是指向类的指针或引用，转换失败会返回 nullptr
4. 在类的转换时，在类层次间进行上行转换（子类指针指向父类指针）时，dynamic_cast 和 static_cast 的效果是一样的。在进行下行转换（父类指针转化为子类指针）时，dynamic_cast 具有类型检查的功能，比 static_cast 更安全。 向下转换的成功与否还与将要转换的类型有关，即要转换的指针指向的对象的实际类型与转换以后的对象类型一定要相同，否则转换失败。在C++中，编译期的类型转换有可能会在运行时出现错误，特别是涉及到类对象的指针或引用操作时，更容易产生错误。Dynamic_cast操作符则可以在运行期对可能产生问题的类型转换进行测试。
5. 使用 dynamic_cast 进行转换的，基类中 <font color="#ff0000">一定要有虚函数，否则编译不通过</font>（类中存在虚函数，就说明它有想要让基类指针或引用指向派生类对象的情况，此时转换才有意义）。这是由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表中，只有定义了虚函数的类才有虚函数表.

#### dynamic_cast impl
https://blog.csdn.net/passion_wu128/article/details/38511957
从以上代码可以看出：只能在有虚函数的类层次之间使用dynamic_cast。要实现dynamic_cast，编译器会在每个含有虚函数的类的虚函数表的前四个字节存放一个指向_RTTICompleteObjectLocator结构的指针，当然还要额外空间存放_RTTICompleteObjectLocator及其相关结构的数据。以上面代码的类C来说：
![avtor](c_pic/CPP-RTTI-POS.jpg)

这个_RTTICompleteObjectLocator就是实现dynamic_cast的关键结构。里面存放了vfptr相对this指针的偏移，构造函数偏移（针对虚拟继承），type_info指针，以及类层次结构中其它类的相关信息。如果是多重继承，这些信息更加复杂。

所以，dynamic_cast的时间和空间代价是相对较高的，在设计时应避免使用。

如果整个工程都不需要dynamic_cast，可以禁用运行时类型信息（vs2008默认是启用的），这样编译器就不会产生_RTTICompleteObjectLocator及相关数据。

————————————————
原文链接：https://blog.csdn.net/passion_wu128/article/details/38511957

## [迭代器失效的情况](https://www.cnblogs.com/fnlingnzb-learner/p/9300073.html)

迭代器失效分三种情况考虑，也是分三种数据结构考虑，分别为数组型，链表型，树型数据结构。

**数组型数据结构**：该数据结构的元素是分配在连续的内存中，insert和erase操作，都会使得删除点和插入点之后的元素挪位置，所以，插入点和删除掉之后的迭代器全部失效，也就是说insert(*iter)(或erase(*iter))，然后在iter++，是没有意义的。解决方法：erase(*iter)的返回值是下一个有效迭代器的值。 iter =cont.erase(iter);

**链表型数据结构**：对于list型的数据结构，使用了不连续分配的内存，删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器.解决办法两种，erase(*iter)会返回下一个有效迭代器的值，或者erase(iter++).

**树形数据结构**： 使用红黑树来存储数据，插入不会使得任何迭代器失效；删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器.erase迭代器只是被删元素的迭代器失效，但是返回值为void，所以要采用erase(iter++)的方式删除迭代器。

注意：经过erase(iter)之后的迭代器完全失效，该迭代器iter不能参与任何运算，包括iter++,*iter


##  成员初始化列表的概念

因为使用成员初始化列表进行初始化的话，会直接使用传入参数的 **拷贝构造函数** 进行初始化，省去了一次执行传入参数的默认构造函数的过程，否则会调用一次传入参数的默认构造函数。所以使用成员初始化列表效率会高一些。

另外，有三种情况是必须使用成员初始化列表进行初始化的：

1. 常量成员的初始化，因为常量成员只能初始化不能赋值
2. 引用类型
3. 没有默认构造函数的对象必须使用成员初始化列表的方式进行初始化

《C++ Primer》中提到在以下三种情况下需要使用初始化成员列表：   

1. 情况一、成员对象需要调有参构造函数； 
2. 情况二、父类部分需要调有参构造函数；
3. 情况三、需要初始化 **const或引用** 成员数据；

原文链接：https://blog.csdn.net/sinat_20265495/article/details/53670644
```cpp
#include <iostream>
using namespace std;
/*第一类情况，成员对象需要调有参构造函数*/
class Test {
public:
  Test(int) { cout << "Test" << endl; }

private:
  int x;
};
class Mytest {
public:
  Mytest() : test(1) { //初始化
    cout << "Mytest" << endl;
  }

private:
  Test test;
};
/*第二类情况，父类部分需要调有参构造函数*/
class Test1 {
public:
  Test1() { cout << "default Test" << endl; };
  Test1(int x) {
    int_x = x;
    cout << "arg Test" << endl;
  };
  void show() { cout << "base int:" << int_x << endl; }

private:
  int int_x;
};
class Mytest1 : public Test1 {
public:
  Mytest1() {
    Test1(10).show(); // 函数内部只是生成临时对象，并不能修改子类对象的父类成员
  }
};

int main(int argc, char *argv[]) {
  Mytest1 test;
  test.show();
  return 0;
}

```

## [完美转发](https://www.jianshu.com/p/b90d1091a4ff)
### forward使用场景
函数模板通用引用场景，对传入的右值，会被推导为值传递，生成临时变量。
导致被转发函数的右值引用绑到左值临时变量而编译出错。     
error: cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
```cpp  hl_lines="9"
#include <iostream>
#include <utility>
using namespace std;

void g(int &i, int &&j) { cout << i++ << j++ << endl; }

template <typename F, typename T1, typename T2>
void flip(F func, T1 &&t1, T2 &&t2) {
    func(t1, std::forward<T2>(t2));
}

int main() {
    int lval = 1;
    flip(g, lval, 2);
    cout << lval << endl;
    return 0;
}

```
std::move()和std::forward()对比

+ std::move执行到右值的无条件转换。就其本身而言，它没有move任何东西。
+ std::forward只有在它的参数绑定到一个右值上的时候，它才转换它的参数到一个右值。
+ std::move和std::forward只不过就是执行类型转换的两个函数；std::move没有move任何东西，std::forward没有转发任何东西。在运行期，它们没有做任何事情。
+ std::forward<T>()不仅可以保持左值或者右值不变，同时还可以保持const、Lreference、Rreference、validate等属性不变；

### 转发失败的场景

* 当模板类型推导失败或者推导出错误类型，完美转发会失败。
* 导致完美转发失败的实参种类有花括号初始化，作为空指针的0或者NULL，仅有声明的整型static const数据成员，模板和重载函数的名字，位域。

```cpp
#include <iostream>
#include <memory>
#include <utility>

using namespace std;

void fun(int &x) { cout << "lvalue ref" << endl; }
void fun(int &&x) { cout << "rvalue ref" << endl; }
void fun(const int &x) { cout << "const lvalue ref" << endl; }
void fun(const int &&x) { cout << "const rvalue ref" << endl; }

template <typename T> void tmpwrap(T &&t) { fun(t); }
template <typename T> void tmpwrap_forward(T &&t) { fun(std::forward<T>(t)); }
struct A {
  A(int &&n) { std::cout << "rvalue overload, n=" << n << "\n"; }
  A(int &n) { std::cout << "lvalue overload, n=" << n << "\n"; }
  A(const int &&n) { std::cout << "const rvalue overload, n=" << n << "\n"; }
  A(const int &n) { std::cout << "const lvalue overload, n=" << n << "\n"; }
};
template <class U> A make_A(U &&u) {
  cout << "no forward:" << endl;
  return A(u);
}
template <class U> A make_A_forward(U &&u) {
  cout << "with forward:" << endl;
  return A(std::forward<U>(u));
}
int main() {
  const int a = 2;
  cout << "1.传入的是rvalue ref。（字面量，函数返回值，lamda函数）" << endl;
  cout << "2.使用完美转义：", tmpwrap(10);
  cout << "3.不使用完美转义：", tmpwrap_forward(10);
  cout << "======================================================" << endl;
  cout << "1.传入的是lvalue re。（所有具名变量，可以取地址的值）" << endl;
  cout << "2.使用farword()：", tmpwrap(a);
  cout << "3.不使用完美转义：", tmpwrap_forward(a);
  cout << "======================================================" << endl;
  cout << "1.传入的是rvalue "
          "ref。（move()告诉编译器将a设置为左值，减少不必要的拷贝）"
       << endl;
  cout << "2.使用farword()：", tmpwrap(move(a));
  cout << "3.不使用完美转义：", tmpwrap_forward(move(a));

  make_A(1);
  make_A(a);
  make_A_forward(1);
  make_A_forward(a);
  return 0;
}



```

## 布局new

布局 new和其他普通的new不同的是，它在括号里多了另外一个参数。  
比如： `pi = new (ptr) int; //placement new`  
括号里的参数是一个指针，它指向一个内存缓冲器，placement new将在这个缓冲器上分配一个对象。Placement new的返回值是这个被构造对象的地址(比如扣号中的传递参数)。placement new主要适用于：在对时间要求非常高的应用程序中，因为这些程序分配的时间是确定的；长时间运行而不被打断的程序；以及执行一个垃圾收集器(garbage collector)。
使用方法  
在很多情况下，placement new的使用方法和其他普通的new有所不同。这里提供了它的使用步骤。  

```cpp
#include <iostream>
#include <new>
using namespace std;
class base {
  int a;

public:
  base(int aa) : a(aa) {}
  base() = default;
  int geta() { return a; }
  ~base() { cout << "~" << endl; }
};

int main() {
  char buffer[100];
  base *p = new (buffer) base(1);
  cout << p->geta() << endl;
  p->~base();
  // delete []buffer;
  return 0;
}
```

第一步  缓存提前分配
为了保证通过placement new使用的缓存区的memory alignmen(内存队列)正确准备，使用普通的new来分配它：
class Task ;
char * buff = new [sizeof(Task)]; //分配内存
(请注意auto或者static内存并非都正确地为每一个对象类型排列，所以，你将不能以placement new使用它们。)  

第二步：对象的分配
在刚才已分配的缓存区调用placement new来构造一个对象。
Task *ptask = new(buff) Task  

第三步：使用
按照普通方式使用分配的对象：
ptask->suspend();
ptask->resume();
//...

第四步：对象的毁灭    
一旦你使用完这个对象，你必须调用它的析构函数来毁灭它。按照下面的方式调用析构函数：
ptask->~Task(); //调用外在的析构函数

第五步：释放     
你可以反复利用缓存并给它分配一个新的对象（重复步骤2，3，4）如果你不打算再次使用这个缓存，你可以象这样释放它：
delete [] buff;
跳过任何步骤就可能导致运行时间的崩溃，内存泄露，以及其它的意想不到的情况。如果你确实需要使用placement new，请认真遵循以上的步骤。

此外，如果在buff 上创建了两个对象，那么调用析构函数的顺序和创建对象的顺序要反过来.
```cpp
classA *p1=new(buffer)classA("22",22);
clasA *p2=new (buffer)classA("221",221);
.
.
p2->~classA();
p1->~classA();
delete[]buffer;
```
