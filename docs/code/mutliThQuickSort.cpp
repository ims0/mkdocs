#include <iostream>
#include <thread>
using namespace std;

const int threadNum = 3;
std::thread threads[threadNum];
int threadCnt = 0;
void print(int V[], int len) {
  for (int i = 0; i < len; i++) {
    printf("%d ", V[i]);
  }
  printf("\n");
}
void quickSort(int arr[], int left, int right) {
  if (left < right) {
    int i = left;
    int j = right;
    int boundary = arr[left];
    while (i < j) {
      while (i < j && arr[j] >= boundary)
        j--;
      if (i < j)
        arr[i++] = arr[j];
      while (i < j && arr[i] < boundary)
        i++;
      if (i < j)
        arr[j--] = arr[i];
    }
    arr[i] = boundary;
    if (threadCnt < threadNum) {
      threads[threadCnt++] = std::thread(quickSort, arr, left, i - 1);
    } else
      quickSort(arr, left, i - 1);

    if (threadCnt < threadNum) {
      threads[threadCnt++] = std::thread(quickSort, arr, i + 1, right);
    } else
      quickSort(arr, i + 1, right);
  }
}
#define ARRAY 49, 38, 65, 97, 76, 13, 27, 49, 1
int main() {
  int arr[] = {ARRAY};
  const int len = sizeof(arr) / sizeof(int);
  print(arr, len);
  int ii = 0;
  threads[threadCnt++] = std::thread(quickSort, arr, ii, len - 1);
  for (int i = 0; i < threadNum; ++i)
    threads[i].join();

  print(arr, len);
  return 0;
}
