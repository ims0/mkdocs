# patteern 

[c语言中文网设计模式](http://c.biancheng.net/view/8462.html)

![avator](pic/design.png)


## 设计模式主要是基于以下的面向对象设计原则。

+ 对接口编程而不是对实现编程。
+ 优先使用对象组合而不是继承。

这些模式可以分为三大类：

+ 创建型模式（Creational Patterns）
+ 结构型模式（Structural Patterns）
+ 行为型模式（Behavioral Patterns）

## J2EE 模式

这些设计模式特别关注表示层。这些模式是由 Sun Java Center 鉴定的。

+ MVC 模式（MVC Pattern）
+ 业务代表模式（Business Delegate Pattern）
+ 组合实体模式（Composite Entity Pattern）
+ 数据访问对象模式（Data Access Object Pattern）
+ 前端控制器模式（Front Controller Pattern）
+ 拦截过滤器模式（Intercepting Filter Pattern）
+ 服务定位器模式（Service Locator Pattern）
+ 传输对象模式（Transfer Object Pattern）


##  面向对象五个基本原则（SOLID）

### ● Single Responsibility Principle：单一职责原则
定义：   
**一个类应该只有一个职责** ，应该仅有一个引起它变化的原因。每一个职责都是变化的一个轴线，如果一个类有一个以上的职责，这些职责就耦合在了一起。
这会导致脆弱的设计。当一个职责发生变化时，可能会影响其它的职责。另外，多个职责耦合在一起，会影响复用性。例如：要实现逻辑和界面的分离。

单一职责原则好处： 

*  类的复杂性降低，实现什么职责都有清晰明确的定义；
*  可读性提高，复杂性降低，那当然可读性提高了； 
* 变更引起的风险降低，变更是必不可少的，如果接口的单一职责做得好，一个接口修改只对相应的实现类有影响，对其他的接口无影响，这对系统的扩展性、维护性都有非常大的帮助。

### ● Interface Segregation Principle：接口隔离原则
**一个类对另外一个类的依赖性应当是建立在最小的接口上的。** 
一个接口代表一个角色，不应当将不同的角色都交给一个接口。没有关系的接口合并在一起，形成一个臃肿的大接口，这是对角色和接口的污染。

隔离：建立单一接口，不要建立臃肿庞大的接口；即接口要尽量细化，同时接口中的方法要尽量少。

### ● Liskov Substitution Principle：里氏替换原则(Is-A)
定义：所有引用基类的地方必须能透明地使用其子类的对象,c++ public 继承

通俗点讲，只要父类能出现的地方子类就可以出现，而且替换为子类也不会产生任何错误或异常，使用者可能根本就不需要知道是父类还是子类。但是，反过来就不行了，有子类出现的地方，父类未必就能适应。
定义中包含的四层含义：

1. 子类必须完全实现父类的方法,public 继承
2. 子类可以有自己的个性
3. 覆盖或实现父类的方法时输入参数可以被放大
        如果父类的输入参数类型大于子类的输入参数类型，会出现父类存在的地方，子类未必会存在，因为一旦把子类作为参数传入，调用者很可能进入子类的方法范畴。
4. 覆写或实现父类的方法时输出结果可以被缩小
      父类的一个方法的返回值是一个类型T，子类的相同方法（重载或覆写）的返回值为S，那么里氏替换原则就要求S必须小于等于T，也就是说，要么S和T是同一个类型，要么S是T的子类。

### ● Dependence Inversion Principle：依赖倒置原则
依赖倒置原则的核心就是要我们 **面向接口编程** ，理解了面向接口编程，也就理解了依赖倒置。

原始定义：   
* ①高层模块不应该依赖低层模块，两者都应该依赖其抽象； 
* ②抽象不应该依赖细节（实现类）
* ③细节应该依赖抽象。

依赖的三种写法：
①构造函数传递依赖对象（构造函数注入）
②Setter方法传递依赖对象（setter依赖注入）
③接口声明依赖对象（接口注入）

使用原则：
依赖倒置原则的本质就是通过抽象（接口或抽象类）使各个类或模块的实现彼此独立，不互相影响，实现模块间的松耦合，我们怎么在项目中使用这个规则呢？只要遵循以下的几个规则就可以：
①每个类尽量都有接口或 **抽象类** ，或者抽象类和接口两者都具备
②变量的表面类型尽量是接口或者是抽象类
③任何类都不应该从具体类派生（只要不超过两层的继承是可以忍受的）
④结合里氏替换原则使用
 
```cpp
#include <iostream>
using namespace std;


class Ireader
{
public:
    virtual string GetContents() = 0;
};

class Book:public Ireader
{
public:
    virtual string GetContents()
    {
        return "从前有座山，山里有座庙，庙里有个小和尚，要听故事";
    }
};

class NewsPaper:public Ireader
{
public:
    virtual string GetContents()
    {
        return "今日要闻";
    }
};

class EBook:public Ireader
{
public:
    virtual string GetContents()
    {
        return "电子书：钢铁是怎样和炼成的";
    }
};

//Mother中使用了Book 我们就说 Mother 依赖于Book。依赖也理解为耦合
class Mother
{
public:
    void tellStory(Ireader * pi)
    {
        cout << pi->GetContents() << endl;
    }

};

int main()
{
    Mother m;
    Book b;
    NewsPaper n;
    EBook e;
    m.tellStory(&b);
    m.tellStory(&n);
    m.tellStory(&e);
    return 0;
}
//原文链接：https://blog.csdn.net/qq_43648751/article/details/105186130
```
 
### ●Open Closed Principle：开闭原则
**可扩展不可修改**   
软件中的对象（类，模块，函数等等）应该 **对于扩展是开放的，但是对于修改是封闭的** ，这意味着一个实体是允许在不改变它的源代码的前提下变更它的行为。该特性在产品化的环境中是特别有价值的，在这种环境中，改变源代码需要代码审查，单元测试以及诸如此类的用以确保产品使用质量的过程。遵循这种原则的代码在扩展时并不发生改变，因此无需上述的过程。

## other 原则

### DRY

Don’t Repeat Yourself。

简单来讲，写代码的时候，如果出现雷同片段，就要想办法把他们提取出来，成为一段独立的代码。这样的抽象，可以保证任何调用这段代码的程序都能得到一致的结果；同时在需要修改时也能保证所有调用处都能获得更新。

### kiss原则
KISS（ Keep It Simple, Stupid）原则——保持简单，愚蠢。    
KISS原则使代码简单、清晰、易于理解。编程语言是为人类所理解的，所以保持编码的简单和直接，让人类理解。保证你的方法尽量很小；每个方法都不应该超过40-50行代码。

每个方法应该只解决一个小问题，而不是实现很复杂的功能。如果你在方法中有很多条件，把它们分解成更小的单独的方法。它不仅更易于阅读和维护，而且可以更快地发现bug。

### YAGNI 
You aren't gonna need it 的缩写。意思是“你将不需要它”。     
YAGNI 是极限编程的一个原则，说的是除非真的有必要，要不然不要添加额外的功能。因为你以为的功能其实你并不需要它。
极限编程的原则提倡尽可能快、尽可能简单的让代码运行起来（do the simplest things that could possibly work, DTSTTCPW）。不要把太多的精力放在抽象上，这样会浪费很多时间，无法实现让软件快速运行起来的目标。
但实际上 YAGNI 和 DRY 这二者存在矛盾。DRY 提倡的是抽象，提取通用的功能、模块。而 YAGNI 则提倡快速运行软件，不要添加额外的功能，不要进行过多的抽象。

### Rules of Three 
三次法则。来自于《重构-改善既有代码设计》的重构原则。   
当第一次做某件事时只管去做；第二次做类似的事情会产生反感，但无论如何还是可以去做；第三次再做类似的事情，你就应该重构。
这个原则很好的解决了 DRY 和 YAGNI 的冲突。当一个功能重复3次及以上时，才着手进行抽象。
之所以加入 Rules of Three 原则，是有下面几个好处：

1. 方便省事。如果一个功能只有在一两个地方用到，那么就没有必要进行抽象。不仅对于开发者来说省事，对于后面接手的人来说，理解代码也更加方便。毕竟经过抽象后的代码理解起来肯定不如抽象前直观
2. 容易发现共性。多一个东西重复多次后，要发现其共性的地方就更容易了
3. 防止过度冗余。同一个功能写多次代码，管理起来非常麻烦。当需要变更时，需要更改多次，非常不方便。所以实际工作中，代码重复最多允许出现一次，再多就需要抽象了。

### [迪米特原则LoD](https://geek-docs.com/design-pattern/design-principle/law-of-demeter.html)

迪米特原则(Law of Demeter, LoD)是面向对象设计原则的一种，也叫 **最少知道原则**      
迪米特原则是1987年秋天由lan holland在美国东北大学一个叫做迪米特的项目设计提出的，它要求一个对象应该对其他对象有最少的了解，所以迪米特法则又叫做最少知识原则。

#### 迪米特原则问题由来
类与类之间的关系越密切，耦合度越大，当一个类发生改变时，对另一个类的影响也越大。

#### 迪米特原则是什么
一个对象应该对其他对象保持最少的了解
这个原理的名称来源于希腊神话中的农业女神，孤独的得墨忒耳

迪米特原则目的是尽量降低类与类之间的耦合。

自从我们接触编程开始，就知道了软件编程的总的原则：低耦合，高内聚。无论是面向过程编程还是面向对象编程，只有使各个模块之间的耦合尽量的低，才能提高代码的复用率。低耦合的优点不言而喻，但是怎么样编程才能做到低耦合呢？那正是迪米特原则要去完成的。

迪米特原则通俗的来讲，就是一个类对自己依赖的类知道的越少越好。也就是说，对于被依赖的类来说，无论逻辑多么复杂，都尽量地的将逻辑封装在类的内部，对外除了提供的public方法，不对外泄漏任何信息。

迪米特原则还有一个更简单的定义：只与直接的朋友通信。

首先来解释一下什么是直接的朋友：每个对象都会与其他对象有耦合关系，只要两个对象之间有耦合关系，我们就说这两个对象之间是朋友关系。耦合的方式很多，依赖、关联、组合、聚合等。其中，我们称出现成员变量、方法参数、方法返回值中的类为直接的朋友，而出现在局部变量中的类则不是直接的朋友。也就是说，陌生的类最好不要作为局部变量的形式出现在类的内部。

将Student 从 Teacher 抽掉，也就达到了 Student 和 Teacher 的解耦，从而符合了迪米特原则。
```cpp
public class Student {
    private Integer id;
    private String name;

    public Student(Integer id, String name) {
        this.id = id;
        this.name = name;
    }
}

public class Teacher {
    public void call(Monitor monitor) {
        monitor.getSize();
    }
}

public class Monitor {
    public void getSize() {
        List<Student> sts = new ArrayList<>();
        for (int i = 0; i < 10; i++) {
            sts.add(new Student(i + 1, "name" + i));
        }
        System.out.println("班级人数" + sts.size());
    }
}
```


### CARP组合/聚合复用原则


组合/聚合复用原则（Composite/Aggregate Reuse Principle）是面向对象设计原则的一种，也叫合成复用原则。组合/聚合复用原则是指尽量使用组合/聚合，不要使用类继承。在一个新的对象里面使用一些已有的对象，使之成为新对象的一部分，新对象通过向这些对象的委派达到复用已有功能的目的。就是说要 **尽量的使用合成和聚合，而不是继承关系达到复用的目的。**   


#### 组合/聚合复用原则是什么
尽量采用组合(contains-a)、聚合(has-a)的方式而不是继承(is-a)的关系来达到软件的复用目的   
组合/聚合复用原则是通过将已有的对象纳入新对象中，作为新对象的成员对象来实现的，新对象可以调用已有对象的功能，从而达到复用。 原则是尽量首先使用合成 / 聚合的方式，而不是使用继承。

```java
public class Person {
    public void talk(String name) {
        System.out.println(name + " say hello");
    }
    public void walk(String name) {
        System.out.println(name + " move");
    }
}

public class Manager extends Person {
}

public class Employee extends Person {
}
```
根据组合/聚合复用原则大家需要首选组合，然后才能是继承，使用继承时需要严格的遵守里氏替换原则，务必满足“Is-A”的关系是才可以使用继承，而组合却是一种“Has-A”的关系。

由上面的代码能够看出，Manager和Employee继承了Person，但实际上每一个不同的职位具有不同的角色，如果大家填加了角色这个类，那麼继续使用继承的话只能使每个人只能具有一种角色，这显然是不科学的。


