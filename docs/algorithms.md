# algorithms

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

## [sort](https://zhuanlan.zhihu.com/p/124356219)

排序是算法研究中最基础的问题

时间复杂度在算法导论中统一是nlogn
![avatar](dataStruct_pic/sort_sum.png)

### 插入排序：直接插入排序
初始有序数列长度为1，紧跟有序数列的一个元素作为要插入的元素，
如果比有序数列的最后一位大，则直接增加数列长度，否则在有序数列中找到合适的位置插入.
```cpp
void insert(int V[], int len) 
{
  for (int curOrderLen = 1; curOrderLen < len; curOrderLen++) {
    int insertItem = V[curOrderLen];
    int j = curOrderLen - 1;  //是有序数组的最后一个元素
    for (; j >= 0 && insertItem < V[j]; j--)
      V[j + 1] = V[j];
    V[j + 1] = insertItem; //找到插入位置J+1,满足>= 前面，<后面
  }
}
```
### 插入排序：希尔排序 

[希尔排序--简单易懂图解](https://blog.csdn.net/qq_39207948/article/details/80006224)
```cpp
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

```cpp
void bubble(vector<int> &vect) {
  int n = vect.size();
  for (int tail = 1; tail < n; tail++) // 每次比较需要两个元素，所以只能比较n-1次
  {
    for (int j = 0; j < n - tail; j++) // j 循环确定了每一趟的比较次数为 n-i
    {
      if (vect[j] > vect[j + 1])
        swap(vect[j], vect[j + 1]);
    }
  }
}
```

### 交换排序：快排

#### 最坏情况
最坏情况是每次划分选取的基准都是当前无序区中最小(或最大)的记录。

1. 数组是正序
2. 数组是倒序
3. 数组值都一样

划分的结果是基准左边的子区间为空(或右边的子区间为空)，而划分所得的另一个非空的子区间中记录数目，仅仅比划分前的无序区中记录个数减少一个。
因此，快速排序必须做n-1次划分，第i次划分开始时区间长度为n-i+1，所需的比较次数为n-i(1≤i≤n-1)，故总的比较次数达到最大值：
`Cmax = n(n-1)/2=Cmax = n(n-1)/2=O(n2)`

#### 快排存在的问题，如何优化

<font color="#0022ff">种快排基准选择方法：</font>

随机（rand函数）、固定（队首、队尾）、三数取中（队首、队中和队尾的中间数）

<font color="#0022ff">4种优化方式：</font>

优化1：当待排序序列的长度分割到一定大小后，使用插入排序

优化2：在一次分割结束后，可以把与Key相等的元素聚在一起，继续下次分割时，不用再对与key相等元素分割

优化3：优化递归操作

优化4：使用并行或多线程处理子序列
```cpp
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
mutli thread code
```cpp
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
```

### 选择排序：直接选择排序
第一层循环变量i做最小索引的初始值，第二层从i+1开始更新最小索引，最后swap

```cpp  linenums="1" hl_lines="7 9"
void select(vector<int> &vect) {
  if (vect.size() < 2) {
    return;
  }
  int n = vect.size();
  for (int i = 0; i < n - 1; ++i) {
    int min = i;
    for (int j = i + 1; j < n; ++j) {
      min = vect[min] > vect[j] ? j : min;
    }
    swap(vect[min], vect[i]);
  }
}
```

### 选择排序：堆排序

heapsort 时间复杂度是O(nlgn),具有空间原址性，常数个额外空间。

堆：二叉堆是一个数组，可以看成完全二叉树，

+ MAX-HEAPIFY: 时间复杂度是 O(lgn),用于维护堆的性质
+ BUILD-MAX-HEAP: 具有线性时间复杂度，把无序数组构造成最大堆。
+ HEAPSORT: 时间 O(nlgn), 原址排序。

#### 优先队列

优先队列不再遵循先入先出的原则，而是分为两种情况：

1. 最大优先队列，无论入队顺序，当前最大的元素优先出队。
2. 最小优先队列，无论入队顺序，当前最小的元素优先出队。

让我们回顾一下二叉堆的特性：

1. 最大堆的堆顶是整个堆中的最大元素
2. 最小堆的堆顶是整个堆中的最小元素

由于堆的上浮下沉时间都是O(lgn),
所以优先队列的入队出队也是O(lgn)

```cpp linenums="1" hl_lines="27 28 32 33"
void heap_print(vector<int> &arr) {
  size_t num = 0;
  for (int layer = 0; num < arr.size(); ++layer) {
    for (int i = 0; i < (1 << layer) && num < arr.size(); ++i)
      printf("%3d ", arr[num++]);
    printf("\n");
  }
}

void build(vector<int> &arr, int s, int e) {
  int dad = s;
  int son = 2 * s + 1;
  while (son <= e) {
    if (son + 1 <= e && arr[son] < arr[son + 1])
      son++;
    if (arr[dad] >= arr[son])
      return;
    else {
      swap(arr[dad], arr[son]);
      dad = son;
      son = son * 2 + 1;
    }
  }
}
void heap(vector<int> &arr) {
  int n = arr.size();
  for (int i = n / 2 - 1; i >= 0; i--) {
    build(arr, i, n - 1);
  }
  heap_print(arr);
  for (int i = n - 1; i > 0; i--) {
    swap(arr[0], arr[i]);
    build(arr, 0, i - 1);
  }
}

```


###  归并排序
![avatar](dataStruct_pic/merge_sort.gif)

```cpp linenums="1" hl_lines="3 10 22"
template<typename T>
void merges(vector<T> &arr, vector<T> &reg, int l, int r) {
  if (l >= r)
    return;
  int mid = l + (r - l) / 2;
  int s1 = l, e1 = mid;
  int s2 = mid + 1, e2 = r;
  merges(arr, reg, s1, e1);
  merges(arr, reg, s2, e2);
  int k = l;
  while (s1 <= e1 && s2 <= e2) {
    reg[k++] = arr[s1] < arr[s2] ? arr[s1++] : arr[s2++];
  }

  while (s1 <= e1) {
    reg[k++] = arr[s1++];
  }

  while (s2 <= e2) {
    reg[k++] = arr[s2++];
  }
  for (int i = l; i <= r; ++i) {
    arr[i] = reg[i];
  }
}
template<typename T>
void merge(vector<T> &arr) {
  vector<T> reg(arr.size(), 0);
  merges(arr, reg, 0, arr.size() - 1);
}
```



###  基数排序


```cpp
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

## 单链表排序

### 冒泡排序

冒泡排序的基本思想就是对于给定的n个元素，从第一个元素开始，依次对相邻的两个元素进行比较，当前面的元素大于后面的元素时，交换其位置，进行一轮比较和换位后，n个元素中最大的数将位于第n位，然后对前（n-1）个元素进行第二轮比较，重复该过程，直到进行比较的元素只剩下一个。

### 快速排序
单链表的快速排序和数组的快速排序在基本细想上是一致的，以从小到大来排序单链表为例，都是选择一个支点，然后把小于支点的元素放到左边，把大于支点的元素放到右边。但是，由于单链表不能像数组那样随机存储，和数组的快排序相比较，还是有一些需要注意的细节：

1. 支点的选取，由于不能随机访问第K个元素，因此每次选择支点时可以取待排序那部分链表的头指针。

2. 遍历链表方式，由于不能从单链表的末尾向前遍历，因此使用两个指针分别向前向后遍历的策略实效，可以可以采用一趟遍历的方式将较小的元素放到单链表的左边。具体方法为：

    1)定义两个指针pSlow, pFast,其中pSlow指单链表头结点，pFast指向单链表头结点的下一个结点;

    2)使用pFast遍历单链表，每遇到一个比支点小的元素，就和pSlow进行数据交换,然后令pSlow=pSlow->next。

3. 交换数据方式，直接交换链表数据指针指向的部分，不必交换链表节点本身。

快排这里还是写了两个函数，Partition这个函数，返回的支点结点的前一个位置。

[singleLinkSort](code/singleLinkSort.cpp)


## 淘汰算法

### LRU
```cpp
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;
template <typename KeyT, typename ValueT> class LRUCache {

public:
  using Pair = std::pair<KeyT, ValueT>;
  using List = std::list<Pair>;
  using Map = std::unordered_map<KeyT, typename List::iterator>;

  void put(KeyT key, ValueT value);
  bool get(KeyT key, ValueT &pValue);//返回值bool

  explicit LRUCache(int cap) : m_capacity(cap){};
  ~LRUCache() {
    m_list.clear();
    m_map.clear();
  };
  LRUCache(const LRUCache &) = delete;
  LRUCache &operator=(const LRUCache &) = delete;

private:
  size_t m_capacity;
  List m_list;//链表存的是key,val
  Map m_map;//key,list::iter
};

template <typename KeyT, typename ValueT>
void LRUCache<KeyT, ValueT>::put(KeyT key, ValueT value) {
  auto iter = m_map.find(key);
  if (iter != m_map.end()) {
    m_list.erase(iter->second);
  }
  m_list.push_front(make_pair(key, value));
  m_map[key] = m_list.begin();
  if (m_list.size() > m_capacity) {
    m_map.erase(m_list.back().first);//map也要删除
    m_list.pop_back(); //列表最后一个元素删除
  }
}
template <typename KeyT, typename ValueT>
bool LRUCache<KeyT, ValueT>::get(KeyT key, ValueT &pValue) {
  auto iter = m_map.find(key);
  if (iter == m_map.end()) {
    return false;
  }
  auto listIt = iter->second;
  m_list.push_front(*listIt);
  m_list.erase(listIt);
  m_map[key] = m_list.begin();
  pValue = listIt->second;
  return true;
}

int main() {
  LRUCache<int, string> lrucache(3);
  lrucache.put(1, "A");
  lrucache.put(2, "B");
  lrucache.put(3, "C");
  std::string value;
  bool ret = lrucache.get(1, value);
  cout << "value=" << value << ",ret=" << ret << endl;
  lrucache.put(4, "D");

  value = "";
  ret = lrucache.get(2, value);
  cout << "value=" << value << ",ret=" << ret << endl;
  return 0;
}
/** out **/
value=A,ret=1
value=,ret=0
```
### LFU
```cpp

```


##  判判断一个链表是否有环，如何找到这个环的起点
给定一个单链表，只给出头指针h：

1. 如何判断是否存在环？
2. 如何知道环的长度L？
3. 如何找出环的连接点在哪里？
4. 带环链表的长度是多少？

### 解法：
1. 问题1，使用追赶的方法，设定两个指针slow、fast，从头指针开始，每次分别前进1步、2步。如存在环，则两者相遇；如不存在环，fast遇到NULL退出。
2. 问题2，记录下问题1的碰撞点p，slow从该点开始，再次回到该点所走过的操作数就是环的长度s。
3. 问题3：有定理：碰撞点p到连接点的距离=头指针到连接点的距离，因此，分别从碰撞点、头指针开始走，相遇的那个点就是连接点。

证明：链表起点到连接点的距离为x，连接点到相遇点的距离为y，圆的周长为L，
前置条件：`x<L` 为了保证相遇时slow还没走一圈，
slow 指针路程：s=x+y，fast指针路程等于`2s = 2(x+y) = x + L + y` 得到 `L=x+y`

4. 问题3中已经求出链表起点到连接点的长度x，加上问题2中求出的环的长度L，二者之和就是带环单链表的长度：x+L


## 判断两个相交链表的交点
同时遍历两个链表到尾部，同时记录两个链表的长度。若两个链表最后的一个节点相同，则两个链表相交。
有两个链表的长度后，我们就可以知道哪个链表长，设较长的链表长度为len1,短的链表长度为len2。
则先让较长的链表向后移动(len1-len2)个长度。然后开始从当前位置同时遍历两个链表，当遍历到的链表的节点相同时，则这个节点就是第一个相交的节点。
![avatar](alg_pic/two_link_join.png)


## 字符串单模式匹配

暴力算法BF算法，利用哈希值进行比较的RK算法，以及尽量减少比较次数的BM算法

### [KMP 算法](https://baijiahao.baidu.com/s?id=1659735837100760934&wfr=spider&for=pc)

#### 思路：
在已匹配的前缀当中寻找到最长可匹配后缀子串和最长可匹配前缀子串，在下一轮直接把两者对齐，从而实现模式串的快速移动。

[kmpCode](code/kmpMatcher.cpp)

### BM算法

[bmCode](code/bmMatcher.cpp)

* BM算法核心思想是，利用模式串本身的特点，在模式串中某个字符与主串不能匹配的时候，将模式串往后多滑动几位，以此来减少不必要的字符比较，提高匹配的效率。
* BM算法构建的规则有两类，**坏字符规则**和**好后缀规则**。
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


