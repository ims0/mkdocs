# algorithms

## 字符串匹配

暴力算法BF算法，利用哈希值进行比较的RK算法，以及尽量减少比较次数的BM算法，

### KMP 算法

Knuth-Morris-Pratt

[kmpCode](code/kmpMatcher.cpp)

### BM算法

[bmCode](code/bmMatcher.cpp)

* BM算法核心思想是，利用模式串本身的特点，在模式串中某个字符与主串不能匹配的时候，将模式串往后多滑动几位，以此来减少不必要的字符比较，提高匹配的效率。
* BM算法构建的规则有两类，坏字符规则和好后缀规则。
* 好后缀规则可以独立于坏字符规则使用。
* 因为坏字符规则的实现比较耗内存，为了节省内存，我们可以只用好后缀规则来实现BM算法。

原文链接：https://blog.csdn.net/qq_21201267/article/details/92799488

### 单匹配模式


### 多匹配模式

