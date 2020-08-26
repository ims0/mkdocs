#include <cstdio>

void print(int V[], int len) {
  for (int i = 0; i < len; i++) {
    printf("%d ", V[i]);
  }
  printf("\n");
}

// 快速排序
void quickSort(int arr[], int left, int right) {
  if (left < right) {
    int i = left;
    int j = right;
    int boundary = arr[left];
    printf("boundary:%d; ", boundary);
    while (i < j) {
      while (i < j && arr[j] >= boundary) // 从右向左找第一个 < boundary的数
        j--;
      if (i < j)
        arr[i++] = arr[j];
      while (i < j && arr[i] < boundary) // 从左向右找第一个 >= boundary的数
        i++;
      if (i < j)
        arr[j--] = arr[i];
    }// 此时 i == j; 且此位置空缺，把边界值填入；
    arr[i] = boundary;
    print(arr, 8);
    quickSort(arr, left, i - 1); // 递归调用;边界值不进入下一轮;
    quickSort(arr, i + 1, right);
  }
}

void bubble(int v[], int n) // 1冒泡排序
{
  for (int i = 1; i < n; i++) // i循环决定了冒泡的趟数是 n-1
  {
    for (int j = 0; j < n - i; j++) // j 循环确定了每一趟的比较次数为 n-i
    {
      if (v[j] > v[j + 1]) {
        v[j] = v[j] ^ v[j + 1];
        v[j + 1] = v[j] ^ v[j + 1];
        v[j] = v[j] ^ v[j + 1];
      }
    }
  }
}

void select_sort(int v[], int len) // 2,选择排序,找出最小的放在最前面
{
  for (int i = 0; i < len - 1; i++) {
    int min = i;
    for (int j = i + 1; j < len; j++)
      min = v[min] > v[j] ? j : min;
    v[i] = v[i] ^ v[min];
    v[min] = v[i] ^ v[min];
    v[i] = v[i] ^ v[min];
  }
}

void insert(int V[], int len) // 3,插入排序
{
  for (int i = 1; i < len; i++) {
    int end = V[i];
    int j;
    for (j = i - 1; end < V[j] && j >= 0;
         j--) //找到合适位置之前把值依次向后移动。
      V[j + 1] = V[j];
    V[j + 1] = end;
  }
}

int maxbit(int data[], int n) //辅助函数，求数据的最大位数
{
  int d = 1; //保存最大的位数
  int p = 10;
  for (int i = 0; i < n; ++i) {
    while (data[i] >= p) {
      p *= 10;
      ++d;
    }
  }
  return d;
}

void radixsort(int data[], int n) //基数排序
{
  int d = maxbit(data, n);
  int *tmp = new int[n];
  int *count = new int[10]; //计数器
  int i, j, k;
  int radix = 1;
  for (i = 1; i <= d; i++) //进行d次排序
  {
    for (j = 0; j < 10; j++)
      count[j] = 0; //每次分配前清空计数器
    for (j = 0; j < n; j++) {
      k = (data[j] / radix) % 10; //统计每个桶中的记录数
      count[k]++;
    }
    for (j = 1; j < 10; j++)
      count[j] = count[j - 1] + count[j]; //将tmp中的位置依次分配给每个桶

    for (j = n - 1; j >= 0; j--) //将所有桶中记录依次收集到tmp中
    {
      k = (data[j] / radix) % 10;
      tmp[count[k] - 1] = data[j];
      count[k]--;
    }
    for (j = 0; j < n; j++) //将临时数组的内容复制到data中
      data[j] = tmp[j];
    radix = radix * 10;
  }
  delete[] tmp;
  delete[] count;
}
void Merge(int arr[], int reg[], int start, int end) {
  if (start >= end)
    return;
  int len = end - start, mid = (len >> 1) + start;

  //分成两部分
  int start1 = start, end1 = mid;
  int start2 = mid + 1, end2 = end;
  //然后合并
  Merge(arr, reg, start1, end1);
  Merge(arr, reg, start2, end2);

  int k = start;
  //两个序列一一比较,哪的序列的元素小就放进reg序列里面,然后位置+1再与另一个序列原来位置的元素比较
  //如此反复,可以把两个有序的序列合并成一个有序的序列
  while (start1 <= end1 && start2 <= end2)
    reg[k++] = arr[start1] < arr[start2] ? arr[start1++] : arr[start2++];

  //然后这里是分情况,如果arr2序列的已经全部都放进reg序列了然后跳出了循环
  //那就表示arr序列还有更大的元素(一个或多个)没有放进reg序列,所以这一步就是接着放
  while (start1 <= end1)
    reg[k++] = arr[start1++];

  //这一步和上面一样
  while (start2 <= end2)
    reg[k++] = arr[start2++];
  //把已经有序的reg序列放回arr序列中
  for (k = start; k <= end; k++)
    arr[k] = reg[k];
}

void MergeSort(int arr[], const int len) {
  //创建一个同样长度的序列,用于临时存放
  int *reg = new int[len];
  Merge(arr, reg, 0, len - 1);
  delete[] reg;
}

void shell_sort(int a[], int n) {
  int gap;
  for (gap = 3; gap > 0; gap--) {
    for (int i = 0; i < gap; i++) {
      for (int j = i + gap; j < n; j = j + gap) {
        if (a[j] < a[j - gap]) {
          int temp = a[j];
          int k = j - gap;
          while (k >= 0 && a[k] > temp) {
            a[k + gap] = a[k];
            k = k - gap;
          }
          a[k + gap] = temp;
        }
      }
    }
  }
}

#define ARRAY 49, 38, 65, 97, 76, 13, 27, 49
int main() {
  int arr[] = {ARRAY};
  const int len = sizeof(arr) / sizeof(int);
  print(arr, len);

  puts("quickSort start");
  quickSort(arr, 0, len - 1);
  print(arr, len);
  puts("quickSort end");

  int a1[] = {ARRAY};
  bubble(a1, len);
  print(a1, len);

  int a2[] = {ARRAY};
  select_sort(a2, len);
  print(a2, len);

  int a3[] = {ARRAY};
  insert(a3, len);
  print(a3, len);

  int a5[] = {ARRAY};
  MergeSort(a5, len);
  print(a5, len);

  int a6[] = {ARRAY};
  radixsort(a6, len);
  print(a6, len);

  int a7[] = {ARRAY};
  shell_sort(a7, len);
  print(a7, len);
}
