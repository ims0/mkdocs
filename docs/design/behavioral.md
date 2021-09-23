## 三，行为型模式
这些设计模式特别关注对象之间的通信。

### 3.1 责任链模式（Chain of Responsibility）
+ 意图  
避免请求发送者与接收者耦合在一起，让多个对象都有可能接收请求，将这些对象连接成一条链，并且沿着这条链传递请求，直到有对象处理它为止。

### 3.2 命令模式（Command）
### 3.3 解释器模式（Interpreter）
### 3.4 迭代器模式（Iterator）
+ 意图  
提供一个方法顺序访问一个聚合对象中的各个元素，而又不需要暴露该对象的内部表示。

### 3.5 中介者模式（Mediator）
### 3.6 备忘录模式（Memento）
### 3.7 观察者模式（Observer）
+ 意图  
定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖它的对象都得到通知并自动更新。
```cpp
class IObserver {
public:
  virtual void Update(const std::string &message_from_subject) = 0;
  virtual ~IObserver(){};
};

class ISubject {
public:
  virtual ~ISubject(){};
  virtual void Attach(IObserver *observer) = 0;
  virtual void Detach(IObserver *observer) = 0;
  virtual void Notify() = 0;
};

class Subject : public ISubject {
public:
  void Attach(IObserver *observer) override {
    list_observer_.push_back(observer);
  }
  void Detach(IObserver *observer) override { list_observer_.remove(observer); }
  void Notify() override {
    std::list<IObserver *>::iterator iterator = list_observer_.begin();
    while (iterator != list_observer_.end()) {
      (*iterator)->Update(message_);
      ++iterator;
    }
  }
  void CreateMessage(std::string message = "Empty") {
    this->message_ = message;
    Notify();
  }

  virtual ~Subject() { std::cout << "Goodbye, I was the Subject.\n"; }

private:
  std::list<IObserver *> list_observer_;
  std::string message_;
};

class Observer : public IObserver {
public:
  Observer(Subject &subject) : subject_(subject) {
    this->subject_.Attach(this);
  }
  virtual ~Observer() {}

  void Update(const std::string &message_from_subject) override {
    message_from_subject_ = message_from_subject;
    std::cout << "a new message is available --> "
              << this->message_from_subject_ << "\n";
  }
  void RemoveMeFromTheList() {
    subject_.Detach(this);
    std::cout << "removed from the list." << endl;
  }

private:
  std::string message_from_subject_;
  Subject &subject_;
};
```

当对象间存在一对多关系时，则使用观察者模式（Observer Pattern）。比如，当一个对象被修改时，则会自动通知依赖它的对象。观察者模式属于行为型模式。
### 3.8 状态模式（State）
+ 意图  
对有状态的对象，把复杂的“判断逻辑”提取到不同的状态对象中，允许状态对象在其内部状态发生改变时改变其行为。

### 3.9 策略模式（Strategy）
+ 意图  
该模式定义了一系列算法，并将每个算法封装起来，使它们可以相互替换，且算法的变化不会影响使用算法的客户。策略模式属于对象行为模式，它通过对算法进行封装，把使用算法的责任和算法的实现分割开来，并委派给不同的对象对这些算法进行管理。
### 3.10 模板模式（Template Method）
+ 意图  
定义一个操作中的算法骨架，而将算法的一些步骤延迟到子类中，使得子类可以不改变该算法结构的情况下重定义该算法的某些特定步骤。它是一种类行为型模式。


### 3.11 访问者模式（Visitor）
+ 意图  
将作用于某种数据结构中的各元素的操作分离出来封装成独立的类，使其在不改变数据结构的前提下可以添加作用于这些元素的新的操作，为数据结构中的每个元素提供多种访问方式。它将对数据的操作与数据结构进行分离，是行为类模式中最复杂的一种模式。


+ 空对象模式（Null Object Pattern）
