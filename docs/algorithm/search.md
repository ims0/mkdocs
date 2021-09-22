## 查找算法

### 二叉查找(lgn)
```cpp linenums="1" hl_lines="7 9"
int binarySearch(vector<int> &arr, int start, int end, int target) {
  while (start <= end) { // 注意
    int mid = (start + end) / 2;
    if (arr[mid] == target)
      return mid;
    else if (arr[mid] < target)
      start = mid + 1; // 注意
    else if (arr[mid] > target)
      end = mid - 1; // 注意
  }

  if (end < 0)
    return start;
  else if (start == (int)arr.size()) {
    return end;
  } else {
    cout << start << ":" << end << endl;
    return arr[start] - target < target - arr[end] ? start : end;
  }
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
```

