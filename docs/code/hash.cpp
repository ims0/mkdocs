/*********************************************************
  > File Name: hash.cpp
  > Author: 10243335
  > Created Time: 2020年06月23日 星期二 09时43分54秒
 *********************************************************/
#include <algorithm>
#include <iostream>
#include <list>
#include <tuple>
#include <vector>
using std::list;
using std::string;
using std::vector;

template <typename Key> class _hash {
public:
  size_t operator()(const Key &k) const;
};
//类模板显式具体化
template <> class _hash<string> {
public:
  size_t operator()(const string &key) {
    size_t hashVal = 0;
    for (char ch : key)
      hashVal = 37 * hashVal + ch;
    return hashVal;
  }
};
template <typename HashedObj> class HashTable {
public:
  explicit HashTable(int size = 101) {
    theLists.resize(size);
    std::cout << theLists.size() << std::endl;
  }
  bool contains(const HashedObj &x) const {
    auto &whichList = theLists[myhash(x)];
    return find(begin(whichList), end(whichList), x) != end(whichList);
  }
  void makeEmpty() {
    for (auto &thisList : theLists)
      thisList.clear();
  }

  bool insert(const HashedObj &x) {
    auto &whichList = theLists[myhash(x)];
    auto itr = std::find(std::begin(whichList), std::end(whichList), x);
    if (itr != std::end(whichList))
      return false;
    whichList.push_back(x);
    ++currentSize;
    return true;
  }
  bool remove(const HashedObj &x) {
    auto &whichList = theLists[myhash(x)];
    auto itr = find(begin(whichList), end(whichList), x);
    if (itr == end(whichList))
      return false;
    whichList.erase(itr);
    --currentSize;
    return true;
  }

  std::tuple<typename std::list<HashedObj>::iterator, bool>
  find(const HashedObj &x) {
    auto &whichList = theLists[myhash(x)];
    const auto &iter = std::find(std::begin(whichList), std::end(whichList), x);
    return make_tuple(iter, iter != end(whichList));
  }

private:
  vector<list<HashedObj>> theLists; // The array of Lists
  size_t currentSize = 0;
  void rehash() {}
  size_t myhash(const HashedObj &x) const {
    static _hash<HashedObj> hf;
    return hf(x) % theLists.size();
  }
};

// Example of an Employee class
class Employee {
public:
  Employee(string _name) : name(_name) {}
  Employee(string _name, double wage) : name(_name), salary(wage) {}
  const string &getName() const { return name; }

  bool operator==(const Employee &rhs) const {
    return getName() == rhs.getName();
  }
  bool operator!=(const Employee &rhs) const { return !(*this == rhs); }
  // Additional public members not shown
  double getSalary() const { return salary; }

private:
  string name;
  double salary = 0.0;
};

template <> class _hash<Employee> {
public:
  size_t operator()(const Employee &item) {
    static _hash<string> hf;
    return hf(item.getName());
  }
};

int main() {

  HashTable<Employee> table(101);
  table.insert(Employee("liming", 1000.1));
  auto iter = table.find(Employee("liming"));
  if (std::get<1>(iter)) {
    auto &employee = std::get<0>(iter);
    std::cout << employee->getName() << ",wage:" << employee->getSalary()
              << std::endl;
  }
  if (std::get<1>(iter = table.find(Employee("wrongName")))) {
    auto &employee = std::get<0>(iter);
    std::cout << employee->getName() << ",wage:" << employee->getSalary()
              << std::endl;
  } else {
    std::cout << "wrongName can not find" << std::endl;
  }
  return 0;
}
