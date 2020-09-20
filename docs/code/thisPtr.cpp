/*********************************************************
    > File Name: this.cpp
    > Author: ims
    > Created Time: Sun Sep 20 08:26:30 2020
 *********************************************************/

#include <iostream>
using namespace std;

class Test {
public:
  void FunTest() {
    cout << this << endl; //can be call when pt == NULL
    //_data = 10;//segmentation fault when pt ==NULL
  }
  static void func() { cout << "static" << endl; }
  int _data;
};

int main() {
  Test t;
  t.FunTest();//lea -0x14(%rbp),%rax; mov %rax,%rdi; callq <Test::FunTest()>
  t.func(); //callq <Test::func()>;
  Test *pt = &t; // lea -0x14(%rbp),%rax;mov  %rax,-0x10(%rbp)
  pt->FunTest(); // //lea -0x14(%rbp),%rax; mov %rax,%rdi; callq <Test::FunTest()>
  pt = NULL; //movq   $0x0,-0x10(%rbp)
  cout << "pt:" << pt << endl;
  pt->FunTest(); //mov  -0x10(%rbp),%rax; mov  %rax,%rdi; callq <Test::FunTest()>
  return 0;
}
