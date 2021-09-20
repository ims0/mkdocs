## 单链表合并
```cpp
#include <iostream>
using namespace std;
class Node {
public:
  int val;
  Node *next = nullptr;
  Node(int x) : val(x) {}
};

void Merge(Node *root, Node *h1, Node *h2) {
  if (!h1) {
    root->next = h2;
    return;
  }
  if (!h2) {
    root->next = h1;
    return;
  }
  if (h1->val < h2->val) {
    root->next = h1;
    Merge(root->next, h1->next, h2);
  } else {
    root->next = h2;
    Merge(root->next, h1, h2->next);
  }
}

Node *Merge(Node *h1, Node *h2) {
  Node *ret = new Node(-1);
  Node *tmp = ret;
  while (h1 && h2) {
    if (h1->val < h2->val) {
      tmp->next = h1;
      h1 = h1->next;
    } else {
      tmp->next = h2;
      h2 = h2->next;
    }
    tmp = tmp->next;
  }
  tmp->next = h1 ? h1 : h2;

  return ret->next;
}

int main() {
  Node *h1 = new Node(1);
  h1->next = new Node(3);
  h1->next->next = new Node(5);
  Node *h2 = new Node(2);
  h2->next = new Node(4);

  Node *ret = new Node(-1);
  //Merge(ret, h1, h2);
  while (ret) {
    cout << ret->val << " ";
    ret = ret->next;
  }
  cout << endl;

  Node *ret2 = Merge(h1, h2);
  while (ret2) {
    cout << ret2->val << ",";
    ret2 = ret2->next;
  }
}
```
