# patteern 

## 设计模式主要是基于以下的面向对象设计原则。

+ 对接口编程而不是对实现编程。
+ 优先使用对象组合而不是继承。

## 设计模式总共有 23 种设计模式
这些模式可以分为三大类：

+ 创建型模式（Creational Patterns）
+ 结构型模式（Structural Patterns）
+ 行为型模式（Behavioral Patterns）

### 创建型模式

这些设计模式提供了一种在创建对象的同时隐藏创建逻辑的方式，而不是使用 new 运算符直接实例化对象。这使得程序在判断针对某个给定实例需要创建哪些对象时更加灵活。

+ 工厂模式（Factory Pattern）
+ 抽象工厂模式（Abstract Factory Pattern）
+ 单例模式（Singleton Pattern）
+ 建造者模式（Builder Pattern）
+ 原型模式（Prototype Pattern）

#### 工厂模式

工厂模式一般分为三种：简单工厂模式、工厂方法模式、抽象工厂模式。

1. 简单工厂模式：

*意图*：定义一个创建对象的接口，让其子类自己决定实例化哪一个工厂类，工厂模式使其创建过程延迟到子类进行。

*主要解决*：主要解决接口选择的问题。

*何时使用*：我们明确地计划不同条件下创建不同实例时。

*如何解决*：让其子类实现工厂接口，返回的也是一个抽象的产品。

*关键代码*：创建过程在其子类执行。

工厂类根据参数不同 创建有公共父类的不同对象，返回父类的指针。例子中是创建不同品牌的车。

```
#include <iostream>
using namespace std;

enum CarType { BENZ, BMW };

class Car {
public:
  virtual void showLogo(void) = 0;
  virtual ~Car() {}
};

class BenzCar : public Car {
public:
  BenzCar() { cout << "Benz::Benz()" << endl; }
  virtual void showLogo(void) { cout << "BenzCar::showLogo()" << endl; }
  ~BenzCar() {}
};

class BmwCar : public Car {
public:
  BmwCar() { cout << "Bmw::Bmw()" << endl; }
  virtual void showLogo(void) { cout << "BmwCar::showLogo()" << endl; }
};

class CarFactory {
public:
  Car *createSpecificCar(CarType type) {
    switch (type) {
    case BENZ:
      return (new BenzCar());
      break;
    case BMW:
      return (new BmwCar());
      break;
    default:
      return NULL;
      break;
    }
  }
};

int main(int argc, char *argv[]) {
  CarFactory carfac;
  Car *carA = carfac.createSpecificCar(BENZ);
  carA->showLogo();
  Car *carB = carfac.createSpecificCar(BMW);
  carB->showLogo();

  delete carA;
  delete carB;
  return 0;
}


```
#### 工厂方法模式：

简述：工厂类改为抽象类，由子类负责创建。

工厂方法模式：不再只由一个工厂类决定那一个产品类应当被实例化,这个决定权被交给子类去做。当有新的产品（新型汽车）产生时，只要按照抽象产品角色、抽象工厂角色提供的方法来生成即可（新车型可以用一个新类继承创建产品即可），那么就可以被客户使用，而不必去修改任何已有的代 码。可以看出工厂角色的结构也是符合开闭原则。

```
#include <iostream>
using namespace std;

class Car {
public:
  virtual void createdCar(void) = 0;
  virtual ~Car() {}
};

class BenzCar : public Car {
public:
  BenzCar() { cout << "Benz::Benz()" << endl; }
  virtual void createdCar(void) { cout << "BenzCar::createdCar()" << endl; }
  ~BenzCar() {}
};

class BmwCar : public Car {
public:
  BmwCar() { cout << "Bmw::Bmw()" << endl; }
  virtual void createdCar(void) { cout << "BmwCar::createdCar()" << endl; }
};

class Factory {
public:
  virtual Car *createSpecificCar(void) = 0;
  virtual ~Factory() {}
};

class BenzFactory : public Factory {
public:
  virtual Car *createSpecificCar(void) { return (new BenzCar()); }
};

class BmwFactory : public Factory {
public:
  virtual Car *createSpecificCar(void) { return (new BmwCar()); }
};

int main(int argc, char **argv) {
  Factory *factory = new BenzFactory();
  Car *specificCarA = factory->createSpecificCar();
  factory = new BmwFactory();
  Car *specificCarB = factory->createSpecificCar();

  delete factory;
  delete specificCarA;
  delete specificCarB;

  return 0;
}
```

#### 抽象工厂：

在上面的工厂方法模式基础上，有需要生产高配版的奔驰和宝马，那工厂方法模式就有点鞭长莫及了，这就又有抽象工厂模式

简述：指定品牌后可以创建多个类型的对象。

```


#include <iostream>
using namespace std;

class Car {
public:
  virtual void createdCar(void) = 0;
  virtual ~Car() {}
};

class BenzCar : public Car {
public:
  BenzCar() { cout << "Benz::Benz()" << endl; }
  virtual void createdCar(void) { cout << "BenzCar::createdCar()" << endl; }
  ~BenzCar() {}
};

class BmwCar : public Car {
public:
  BmwCar() { cout << "Bmw::Bmw()" << endl; }
  virtual void createdCar(void) { cout << "BmwCar::createdCar()" << endl; }
};

class HighCar //高配版车型
{
public:
  virtual void createdCar(void) = 0;
  virtual ~HighCar() {}
};

class HighBenzCar : public HighCar {
public:
  HighBenzCar() { cout << "HighBenzCarBenz::Benz()" << endl; }
  virtual void createdCar(void) { cout << "HighBenzCar::createdCar()" << endl; }
};

class HighBmwCar : public HighCar {
public:
  HighBmwCar() { cout << "HighBmwCar::Bmw()" << endl; }
  virtual void createdCar(void) { cout << "HighBmwCar::createdCar()" << endl; }
};

class Factory {
public:
  virtual Car *createSpecificCar(void) = 0;
  virtual HighCar *createdSpecificHighCar(void) = 0;
  virtual ~Factory() {}
};

class BenzFactory : public Factory {
public:
  virtual Car *createSpecificCar(void) { return (new BenzCar()); }

  virtual HighCar *createdSpecificHighCar(void) { return (new HighBenzCar()); }
};

class BmwFactory : public Factory {
public:
  virtual Car *createSpecificCar(void) { return (new BmwCar()); }
  virtual HighCar *createdSpecificHighCar(void) { return (new HighBmwCar()); }
};

int main(int argc, char **argv) {
  Factory *factory = new BenzFactory();
  Car *specificCar = factory->createSpecificCar();
  HighCar *spcificHighCar = factory->createdSpecificHighCar();

  delete factory;
  delete specificCar;
  delete spcificHighCar;

  return 0;
}

```

#### 单例模式

##### 饿汉模式

是指单例实例在程序运行时被立即执行初始化:
这种模式的问题也很明显, 类现在是多态的, 但静态成员变量初始化顺序还是没保证.

```

#include <iostream>
class ClassStaticObj {
public:
  static ClassStaticObj &Instance() { return m_instance; }
  void Print() { std::cout << m_var << std::endl; }

private:
  ClassStaticObj(){};                                // ctor is hidden
  ClassStaticObj(ClassStaticObj const &);            // copy ctor is hidden
  ClassStaticObj &operator=(ClassStaticObj const &); // assign op is hidden

  int m_var = 10;
  static ClassStaticObj m_instance;
};
// in log. cpp we have to declare
ClassStaticObj ClassStaticObj::m_instance;

int main() {
  ClassStaticObj::Instance().Print();
  return 0;
}

```

##### 懒汉模式(堆-粗糙版)

(堆-粗糙版): 单例实例只在第一次被使用时进行初始化:

Instance() 只在第一次被调用时为 m_pInstance 分配内存并初始化. 嗯, 看上去所有的问题都解决了, 初始化顺序有保证, 多态也没问题. 程序退出时, 析构函数没被执行. 这在某些设计不可靠的系统上会导致资源泄漏, 比如文件句柄, socket 连接, 内存等等 对于这个问题, 比较土的解决方法是, 给每个 Singleton 类添加一个 destructor() 方法:

```
#include <iostream>

class HeapObj {
public:
  static HeapObj *Instance() {
    if (!m_pInstance)
      m_pInstance = new HeapObj;
    return m_pInstance;
  }
  void Print() { std::cout << m_var << std::endl; }

private:
  HeapObj(){};                                // ctor is hidden
  HeapObj(HeapObj const &);            // copy ctor is hidden
  HeapObj &operator=(HeapObj const &); // assign op is hidden

  int m_var = 9;
  static HeapObj *m_pInstance;
};
HeapObj *HeapObj::m_pInstance = NULL;

int main() {
  HeapObj::Instance()->Print();
  return 0;
}

```

##### 懒汉模式 (局部静态变量-最佳版)o

在 Instance() 函数内定义局部静态变量的好处是, theLog 的构造函数只会在第一次调用Instance() 时被初始化, 达到了和 "堆栈版" 相同的动态初始化效果, 保证了成员变量和 Singleton 本身的初始化顺序.

它还有一个潜在的安全措施, Instance() 返回的是对局部静态变量的引用, 如果返回的是指针, Instance() 的调用者很可能会误认为他要检查指针的有效性, 并负责销毁. 构造函数和拷贝构造函数也私有化了, 这样类的使用者不能自行实例化.

```
#include <iostream>
#include <list>
class Log {
public:
  static Log &Instance() {
    static Log singletonLog;
    return singletonLog;
  }
  void Write(char const *logLine) { m_data.push_back(logLine); };
  void Print() {
    for (auto &line : m_data) {
      std::cout << line << std::endl;
    }
  }

private:
  Log(){};                     // ctor is hidden
  Log(Log const &);            // copy ctor is hidden
  Log &operator=(Log const &); // assign op is hidden
  std::list<std::string> m_data;
};
int main() {
  Log::Instance().Write("hello");
  Log::Instance().Print();
  return 0;
}
```



### 结构型模式
这些设计模式关注类和对象的组合。继承的概念被用来组合接口和定义组合对象获得新功能的方式。

+ 适配器模式（Adapter Pattern）
+ 桥接模式（Bridge Pattern）
+ 过滤器模式（Filter、Criteria Pattern）
+ 组合模式（Composite Pattern）
+ 装饰器模式（Decorator Pattern）
+ 外观模式（Facade Pattern）
+ 享元模式（Flyweight Pattern）
+ 代理模式（Proxy Pattern）

### 行为型模式
这些设计模式特别关注对象之间的通信。

+ 责任链模式（Chain of Responsibility Pattern）
+ 命令模式（Command Pattern）
+ 解释器模式（Interpreter Pattern）
+ 迭代器模式（Iterator Pattern）
+ 中介者模式（Mediator Pattern）
+ 备忘录模式（Memento Pattern）
+ 观察者模式（Observer Pattern）
+ 状态模式（State Pattern）
+ 空对象模式（Null Object Pattern）
+ 策略模式（Strategy Pattern）
+ 模板模式（Template Pattern）
+ 访问者模式（Visitor Pattern）

### J2EE 模式

这些设计模式特别关注表示层。这些模式是由 Sun Java Center 鉴定的。

+ MVC 模式（MVC Pattern）
+ 业务代表模式（Business Delegate Pattern）
+ 组合实体模式（Composite Entity Pattern）
+ 数据访问对象模式（Data Access Object Pattern）
+ 前端控制器模式（Front Controller Pattern）
+ 拦截过滤器模式（Intercepting Filter Pattern）
+ 服务定位器模式（Service Locator Pattern）
+ 传输对象模式（Transfer Object Pattern）
