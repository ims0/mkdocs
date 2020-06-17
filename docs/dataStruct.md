[toc]


# data Struct

## 二叉搜索树BST(左小右大)
*Binary Search Tree(不保证平衡)*

二叉搜索树的基本操作花费时间和树高度成正比，对于n个节点的完全二叉树来说，基本操作时间位O(lgn)。 最坏情况是不分叉的单链，时间是 O(n). 随机二叉树期望高度是O(lg n).

### BST树定义
+ 设 x 为二叉搜索树的一个节点，如果 y 是 x 左子树的一个节点，那么 y.key <= x.key； 如果 z 是 x 右子树的一个节点，那么 z.key >= x.key

### [BSTCode](cpp_code/BST.cpp)
### 查询二叉搜索树

####  递归方式

```
TREE_SEARCH(x,k)
if x == NIL or k == x.key
    return x
if k < x.key
    return TREE_SEARCH(x.left,k)
else return TREE_SEARCH(x.left,k)
```
####  循环 

```
ITER_TREE_SEARCH(x,k)
while x != NIL && k != x.key
    if k < x.key
        x == x.left
    else 
        x = x.right
return x
```
#### 查最小值
根据二叉树的性质，最小值在最左下方
```
TREE_MIN(x)
while x.left != NIL
    x = x.left
return x
```

### 前序遍历DLR
*根结点 ---> 左子树 ---> 右子树*
```
void front(Tree* node)  //前序遍历  
{  
if(node){  
    cout << node->val <<"  ";  
    front(node->lchild);  
    front(node->rchild);  
}  
}  
```

### 中序遍历LDR
*左子树---> 根结点 ---> 右子树*

```
void middle(Tree* node)  //中序遍历  
{  
if(node){  
    middle(node->lchild);  
    cout<<node->val<<"  ";  
    middle(node->rchild);  
}  
}  
```
### 后序遍历LRD
*左子树 ---> 右子树 ---> 根结点*

```
void back(Tree* node)  //后序遍历 
{  
if(node){  
    back(node->lchild);  
    back(node->rchild);  
    cout<<node->val<<"  ";   
}  
}  
```


## 自平衡二叉查找树AVL
*Adelson-Velsky-Landis Tree*

### AVL树定义

AVL树本质上是一颗二叉查找树，但是它又具有以下特点：
+ 它是一棵空树或它的左右两个子树的高度差的绝对值不超过1，并且左右两个子树都是一棵平衡二叉树。
+ 在AVL树中任何节点的两个子树的高度最大差别为一，所以它也被称为平衡二叉树。下面是平衡二叉树和非平衡二叉树对比的例图：

![image](dataStruct_pic/avl_1.png)

### AVL树的作用

我们知道，对于一般的二叉搜索树（Binary Search Tree），其期望高度（即为一棵平衡树时）为log2n，其各操作的时间复杂度（O(log2n)）同时也由此而决定。但是，在某些极端的情况下（如在插入的序列是有序的时），二叉搜索树将退化成近似链或链，此时，其操作的时间复杂度将退化成线性的，即O(n)。
![image](dataStruct_pic/avl_2.png)

我们可以通过随机化建立二叉搜索树来尽量的避免这种情况，但是在进行了多次的操作之后，由于在删除时，我们总是选择将待删除节点的后继代替它本身，这样就会造成总是右边的节点数目减少，以至于树向左偏沉。这同时也会造成树的平衡性受到破坏，提高它的操作的时间复杂度。
　　例如：我们按顺序将一组数据1,2,3,4,5,6分别插入到一颗空二叉查找树和AVL树中，插入的结果如下图：

![image](dataStruct_pic/avl_3.png)

由上图可知，同样的结点，由于插入方式不同导致树的高度也有所不同。特别是在带插入结点个数很多且正序的情况下，会导致二叉树的高度是O(N)，而AVL树就不会出现这种情况，树的高度始终是O(lgN).高度越小，对树的一些基本操作的时间复杂度就会越小。这也就是我们引入AVL树的原因.

### AVL树的基本操作：

AVL树的操作基本和二叉查找树一样，这里我们关注的是两个变化很大的操作：插入和删除！

　　我们知道，AVL树不仅是一颗二叉查找树，它还有其他的性质。如果我们按照一般的二叉查找树的插入方式可能会破坏AVL树的平衡性。同理，在删除的时候也有可能会破坏树的平衡性，所以我们要做一些特殊的处理，包括：单旋转和双旋转！

#### AVL树的插入，单旋转的第一种情况---右旋：

![image](dataStruct_pic/avl_4.png)

由上图可知：在插入之前树是一颗AVL树，而插入之后结点T的左右子树高度差的绝对值不再 < 1,此时AVL树的平衡性被破坏，我们要对其进行旋转。由上图可知我们是在结点T的左结点的左子树上做了插入元素的操作，我们称这种情况为左左情况，我们应该进行右旋转(只需旋转一次，故是单旋转)。具体旋转步骤是：

　　T向右旋转成为L的右结点，同时，Y放到T的左孩子上。这样即可得到一颗新的AVL树，旋转过程图如下：
![image](dataStruct_pic/avl_5.png)
　左左情况的右旋举例：
![image](dataStruct_pic/avl_6.png)

#### AVL树的插入，单旋转的第二种情况---左旋：

![image](dataStruct_pic/avl_7.png)

由上图可知：在插入之前树是一颗AVL树，而插入之后结点T的左右子树高度差的绝对值不再 < 1,此时AVL树的平衡性被破坏，我们要对其进行旋转。由上图可知我们是在结点T的右结点的右子树上做了插入元素的操作，我们称这种情况为右右情况，我们应该进行左旋转(只需旋转一次，故事单旋转)。具体旋转步骤是：

　　　T向右旋转成为R的左结点，同时，Y放到T的左孩子上。这样即可得到一颗新的AVL树，旋转过程图如下：
![image](dataStruct_pic/avl_8.png)
　右右情况的左旋举例:
![image](dataStruct_pic/avl_9.png)

以上就是插入操作时的单旋转情况！我们要注意的是：谁是T谁是L，谁是R还有谁是X,Y,Z!T始终是开始不平衡的左右子树的根节点。显然L是T的左结点，R是T的右节点。X、Y、Y是子树当然也可以为NULL.NULL归NULL，但不能破坏插入时我上面所说的左左情况或者右右情况。

#### AVL树的插入，双旋转的第一种情况---左右(先左后右)旋：
![image](dataStruct_pic/avl_10.png)
由上图可知，我们在T结点的左结点的右子树上插入一个元素时，会使得根为T的树的左右子树高度差的绝对值不再 < 1，如果只是进行简单的右旋，得到的树仍然是不平衡的。我们应该按照如下图所示进行二次旋转：

![image](dataStruct_pic/avl_11.png)
　左右情况的左右旋转实例：

![image](dataStruct_pic/avl_12.png)
#### AVL树的插入，双旋转的第二种情况---右左(先右后左)旋：
![image](dataStruct_pic/avl_13.png)

　　由上图可知，我们在T结点的右结点的左子树上插入一个元素时，会使得根为T的树的左右子树高度差的绝对值不再 < 1，如果只是进行简单的左旋，得到的树仍然是不平衡的。我们应该按照如下图所示进行二次旋转：
![image](dataStruct_pic/avl_14.png)

　右左情况的右左旋转实例：
![image](dataStruct_pic/avl_15.png)

### AVL树的插入代码实现:

[avl tree code](cpp_code/AVLTree.cpp)

## RB树
https://www.jianshu.com/p/e136ec79235c

最坏基本动态操作集合时间是 O(lg n)
### 红黑树的性质

红黑树是一种含有红黑结点并能自平衡的二叉查找树。它必须满足下面性质：

+ 性质1：每个节点要么是黑色，要么是红色。
+ 性质2：根节点是黑色。
+ 性质3：每个叶子节点（NIL）是黑色。
+ 性质4：每个红色结点的两个子结点一定都是黑色。
+ 性质5：任意一结点到每个叶子结点的路径都包含数量相同的黑结点。

从性质5又可以推出：

+ 性质5.1：如果一个结点存在黑子结点，那么该结点肯定有两个子结点

图1就是一颗简单的红黑树。其中Nil为叶子结点，并且它是黑色的。(值得提醒注意的是，在Java中，叶子结点是为null的结点。)
