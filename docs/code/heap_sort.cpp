/*********************************************************
    > File Name: heap.cpp
    > Author: ims
    > Created Time: Thu Aug 27 20:23:43 2020
 *********************************************************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int heap_verify(int arr[], int size) {
  for (int i = 0; i < size / 2; ++i) {
    int lhs = 2 * i + 1;
    int rhs = 2 * i + 2;

    if (lhs < size && arr[i] > arr[lhs]) {
      printf("arr[%d]:%d > arr[%d]:%d\n", i, arr[i], lhs, arr[lhs]);
      return -1;
    }
    if (rhs < size && arr[i] > arr[rhs]) {
      printf("arr[%d]:%d > arr[%d]:%d\n", i, arr[i], rhs, arr[rhs]);
      return -1;
    }
  }
  return 0;
}

static void heap_print(int arr[], int size) {
  int num = 0;
  for (int layer = 0; num < size; ++layer) {
    for (int i = 0; i < (1 << layer) && num < size; ++i)
      printf("%3d ", arr[num++]);
    printf("\n");
  }
}

void swap(int *a, int *b) {
  int temp = *b;
  *b = *a;
  *a = temp;
}

//build max heap
void max_heapify(int arr[], int start, int end) {
  //建立父节点指标和子节点指标
  int dad = start;
  int son = dad * 2 + 1;
  while (son <= end) //若子节点指标在范围内才做比较
  {
    if (son + 1 <= end && arr[son] < arr[son + 1])
      //先比较两个子节点大小，选择最大的
      son++;
    if (arr[dad] > arr[son]) //如果父节点大於子节点代表调整完毕，直接跳出函数
      return;
    else //否则交换父子内容再继续子节点和孙节点比较
    {
      swap(&arr[dad], &arr[son]);
      dad = son;
      son = dad * 2 + 1;
    }
  }
}

void heap_sort(int arr[], int len) {
  //初始化，i从最後一个父节点开始调整
  for (int i = len / 2 - 1; i >= 0; i--)
    max_heapify(arr, i, len - 1);
  //先将第一个元素和已排好元素前一位做交换，再重新调整，直到排序完毕
  for (int i = len - 1; i > 0; i--) {
    swap(&arr[0], &arr[i]);
    max_heapify(arr, 0, i - 1);
  }
}

int main() {
  int arr[] = {49, 38, 65, 97, 76, 13, 27};
  const int len = sizeof(arr) / sizeof(int);
  heap_print(arr, len);
  heap_sort(arr, len);
  heap_print(arr, len);
  return 0;
}
