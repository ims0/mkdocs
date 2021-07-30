#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
  int val;
  struct ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};
class List {
public:
  ListNode *CreatList(int *arr, int len) {
    ListNode *pHead = new ListNode(arr[0]);
    ListNode *rear = pHead;
    int count = 1;
    while (count < len) {
      ListNode *pCurrent = new ListNode(arr[count]);
      rear->next = pCurrent;
      rear = pCurrent;
      count++;
    }
    rear->next = NULL;
    return pHead;
  }
  void ShowList(ListNode *pHead) {
    while (pHead) {
      cout << pHead->val << " ";
      pHead = pHead->next;
    }
    cout << endl;
  }
  ListNode *GetLastNode(ListNode *pHead) {
    ListNode *pNode = pHead;
    while (pNode->next != NULL) {
      pNode = pNode->next;
    }
    return pNode;
  }
};
class Sort {
public:
  ListNode *BubbleSortList(ListNode *pHead) {
    ListNode *pNode1 = pHead;
    ListNode *pNode2 = pHead;
    if (pHead == NULL)
      return NULL;
    for (; pNode1->next != NULL; pNode1 = pNode1->next) {
      for (pNode2 = pHead; pNode2->next != NULL; pNode2 = pNode2->next) {
        if (pNode2->val > pNode2->next->val) {
          int temp = pNode2->val;
          pNode2->val = pNode2->next->val;
          pNode2->next->val = temp;
        }
      }
    }
    return pHead;
  }
  void QuickSortList(ListNode *pHead, ListNode *pEnd) {
    if (pHead != pEnd) {
      ListNode *part = Partition(pHead, pEnd);
      QuickSortList(pHead, part);
      QuickSortList(part->next, pEnd);
    }
  }
  ListNode *Partition(ListNode *pBegin, ListNode *pEnd) {

    int key = pBegin->val;
    ListNode *pSlow = pBegin;
    ListNode *pFast = pSlow->next;
    ListNode *temp = pBegin;

    while (pFast != NULL && pFast != pEnd->next) {
      if (pFast->val < key) {
        temp = pSlow;
        pSlow = pSlow->next;

        swap(pSlow->val, pFast->val);
      }

      pFast = pFast->next;
    }
    swap(pSlow->val, pBegin->val);
    return temp; //返回的结点为支点节点的前一个结点    　
  }
};

int main() {
  int array[] = {3, 4, 5, 1, 2, 8, 7};
  List list;
  Sort sort;
  ListNode *pHead1 = list.CreatList(array, sizeof(array) / sizeof(array[0]));
  ListNode *pHead2 = list.CreatList(array, sizeof(array) / sizeof(array[0]));
  list.ShowList(pHead1);
  sort.BubbleSortList(pHead1);
  list.ShowList(pHead1);
  ListNode *pEnd = list.GetLastNode(pHead2);
  // cout<<pEnd->val<<endl;
  sort.QuickSortList(pHead2, pEnd);
  list.ShowList(pHead2);
  return 0;
}
