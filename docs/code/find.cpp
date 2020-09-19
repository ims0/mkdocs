/*********************************************************
    > File Name: find.cpp
    > Author: ims
    > Created Time: Sat Sep 19 19:30:35 2020
 *********************************************************/

#include <iostream>
using namespace std;

int binarySearch(int arr[], int start, int end, int target) {
  while (start <= end) { // 注意
    int mid = (start + end) / 2;
    if (arr[mid] == target)
      return mid;
    else if (arr[mid] < target)
      start = mid + 1; // 注意
    else if (arr[mid] > target)
      end = mid - 1; // 注意
  }
  return -1;
}

int BinSearch(int a[], int start, int end, int val) {
  if (start <= end) // 相等的情况返回，返回相等的值
  {
    int mid = (start + end) / 2;
    if (a[mid] == val)
      return mid;
    else if (a[mid] > val)
      return BinSearch(a, start, mid - 1, val);
    else if (a[mid] < val)
      return BinSearch(a, mid + 1, end, val);
  }
  return -1;
}
int main() {
  int arr[] = {1, 2, 3, 4, 5};
  int len = sizeof(arr) / sizeof(int);
  cout << BinSearch(arr, 0, len - 1, 2) << endl;
  cout << binarySearch(arr, 0, len - 1, 2) << endl;

  cout << BinSearch(arr, 0, len - 1, 1) << endl;
  cout << binarySearch(arr, 0, len - 1, 1) << endl;
  return 0;
}
