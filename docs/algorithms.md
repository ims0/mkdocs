# algorithms

## [sort](https://zhuanlan.zhihu.com/p/124356219)

排序是算法研究中最基础的问题

时间复杂度在算法导论中统一是nlogn
![avatar](dataStruct_pic/sort_sum.png)

### 插入排序：直接插入排序
初始有序数列长度为1，紧跟有序数列的一个元素作为要插入的元素，
如果比有序数列的最后一位大，则直接增加数列长度，否则在有序数列中找到合适的位置插入.
```
void insert(int V[], int len) 
{
  for (int curOrderLen = 1; curOrderLen < len; curOrderLen++) {
    int insertItem = V[curOrderLen];
    int j = curOrderLen - 1;  //是有序数组的最后一个元素
    for (; insertItem < V[j] && j >= 0; j--)
      V[j + 1] = V[j];
    V[j + 1] = insertItem; //找到插入位置J+1,满足>= 前面，<后面
  }
}
```
### 插入排序：希尔排序 

[希尔排序--简单易懂图解](https://blog.csdn.net/qq_39207948/article/details/80006224)
```
//设置不同的gap，调用插入排序
void insert_with_gap(int V[], int len, int gap) 
{
  for (int curOrderLen = gap; curOrderLen < len; curOrderLen++) {
    int insertItem = V[curOrderLen];
    int j = curOrderLen - gap; //是有序数组的最后一个元素
    for (; insertItem < V[j] && j >= 0; j -= gap)
      V[j + gap] = V[j];
    V[j + gap] = insertItem; //找到插入位置J+1,满足>= 前面，<后面
  }
}

void shell_(int arr[], int n) {
  for (int gap = n / 2; gap > 0; gap /= 2) { // gap: n/2 ~ 1
    insert_with_gap(arr, n, gap);
  }
}

// 希尔排序合成版
void shell(int arr[], int n) {
  for (int gap = n / 2; gap > 0; gap /= 2) { // gap: n/2 ~ 1

    for (int i = gap; i < n; i++) { // 2
      int j, insertItem = arr[i];
      for (j = i - gap; j >= 0 && insertItem < arr[j]; j -= gap) {
        arr[j + gap] = arr[j];
      }
      arr[j + gap] = insertItem;
    }
  }
}
```
### 交换排序：冒泡

大数向后移动
```
void bubble(int v[], int n) 
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

```

### 交换排序：快排

```

void quickSort(int arr[], int left, int right) {
  if (left < right) {
    int i = left;
    int j = right;
    int boundary = arr[left];
    // printf("boundary:%d; ", boundary);
    while (i < j) {
      while (i < j && arr[j] >= boundary) // 从右向左找第一个 < boundary的数
        j--;
      if (i < j)
        arr[i++] = arr[j];
      while (i < j && arr[i] < boundary) // 从左向右找第一个 >= boundary的数
        i++;
      if (i < j)
        arr[j--] = arr[i];
    } // 此时 i == j; 且此位置空缺，把边界值填入；
    arr[i] = boundary;
    // print(arr, 8);
    quickSort(arr, left, i - 1); // 递归调用;边界值不进入下一轮;
    quickSort(arr, i + 1, right);
  }
}

```

### 选择排序：直接选择排序
在剩余无序队列中，找出最小的元素放在最有序队列的末尾，逐步扩大有序队列长度
```
void select_sort(int v[], int len) 
{
  for (int i = 0; i < len - 1; i++) {
    int min = i;
    for (int j = i + 1; j < len; j++)
      min = v[min] > v[j] ? j : min;

    int temp = v[i];
    v[i] = v[min];
    v[min] = temp;
  }
}
```
### 选择排序：堆排序

```
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

```


###  归并排序
![avatar](dataStruct_pic/merge_sort.gif)

```
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

```



###  基数排序


```
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
```

### [计数排序](https://www.cnblogs.com/xiaochuan94/p/11198610.html)
用辅助数组，计算每个元素出现的次数，在把计数数组展开到目的数组

### [桶排序](https://www.cnblogs.com/bqwzx/p/11029264.html)

当然桶排序更是对计数排序的改进，计数排序申请的额外空间跨度从最小元素值到最大元素值，若待排序集合中元素不是依次递增的，则必然有空间浪费情况。桶排序则是弱化了这种浪费情况，将最小值到最大值之间的每一个位置申请空间，更新为最小值到最大值之间每一个固定区域申请空间，尽量减少了元素值大小不连续情况下的空间浪费情况。

作者：zhipingChen
链接：https://www.jianshu.com/p/204ed43aec0c


## 字符串单模式匹配

暴力算法BF算法，利用哈希值进行比较的RK算法，以及尽量减少比较次数的BM算法

### [KMP 算法](https://baijiahao.baidu.com/s?id=1659735837100760934&wfr=spider&for=pc)

#### 思路：
在已匹配的前缀当中寻找到最长可匹配后缀子串和最长可匹配前缀子串，在下一轮直接把两者对齐，从而实现模式串的快速移动。

[kmpCode](code/kmpMatcher.cpp)

### BM算法

[bmCode](code/bmMatcher.cpp)

* BM算法核心思想是，利用模式串本身的特点，在模式串中某个字符与主串不能匹配的时候，将模式串往后多滑动几位，以此来减少不必要的字符比较，提高匹配的效率。
* BM算法构建的规则有两类，坏字符规则和好后缀规则。
* 好后缀规则可以独立于坏字符规则使用。
* 因为坏字符规则的实现比较耗内存，为了节省内存，我们可以只用好后缀规则来实现BM算法。

原文链接：https://blog.csdn.net/qq_21201267/article/details/92799488


## 多匹配模式


### trie树

根节点不包含字符，除根节点外每一个节点都只包含一个字符； 从根节点到某一节点，路径上经过的字符连接起来，为该节点对应的字符串； 每个节点的所有子节点包含的字符都不相同。

每一个节点u都包含next[v],value。

next[v]表示节点u的v边指向的节点编号。

value表示危险节点所属的模式串编号，若value=0表示这不是一个危险节点。

根节点表示空串。
原文链接：https://blog.csdn.net/xmr_pursue_dreams/article/details/80287410

### trie图

trie图的目的是让每一次都精确地跳转。

### AC自动机
