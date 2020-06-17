#include<iostream>
#include<fstream>
#include<queue>

using namespace std;

enum RbColor { RED, BLACK };
struct TreeNode {
int key;
RbColor color;
struct TreeNode *p;
struct TreeNode *left;
struct TreeNode *right;
};
struct Tree {
TreeNode *root;
TreeNode *nil;
};

TreeNode gNilNode;
void front(TreeNode *node) //前序遍历
{
if (node != &gNilNode) {
cout << node->key << " ";
front(node->left);
front(node->right);
}
}

void middle(TreeNode *node) //中序遍历
{
if (node != &gNilNode) {
middle(node->left);
cout << node->key << " ";
middle(node->right);
}
}

void back(TreeNode *node) //后序遍历
{
if (node != &gNilNode) {
back(node->left);
back(node->right);
cout << node->key << " ";
}
}

void RbBackDelete(TreeNode *node) {
if (node != &gNilNode) {
RbBackDelete(node->left);
RbBackDelete(node->right);
delete node;
}
}

TreeNode *Minimun(TreeNode *pNode) {
while (pNode->left != &gNilNode) {
pNode = pNode->left;
}
return pNode;
}

TreeNode *Maximun(TreeNode *pNode) {
while (pNode->right != &gNilNode) {
pNode = pNode->right;
}
return pNode;
}

TreeNode *RbSearch(TreeNode *pNode, int key) {
if ((pNode == &gNilNode) || (pNode->key == key))
return pNode;
if (key < pNode->key)
return RbSearch(pNode->left, key);
else
return RbSearch(pNode->right, key);
}

TreeNode *RbSearchLoop(TreeNode *pNode, int key) {
while (pNode != &gNilNode && pNode->key != key) {
if (key < pNode->key)
pNode = pNode->left;
else
pNode = pNode->right;
}
return pNode;
}

void produceGraphviz(Tree *tree, const string fileName) {
if (tree->root == tree->nil)
return;
std::ofstream ofs(fileName);
ofs << "digraph {" << endl;
queue<TreeNode *> q;
q.push(tree->root);
while (!q.empty()) {
TreeNode *tem = q.front();
q.pop();
ofs << tem->key;
if (tem->color == RED)
ofs << "[shape=box,style=filled,color=red]" << endl;
else
ofs << "[shape=box,style=filled,color=blue]" << endl;
if (tem->left != tree->nil) {
ofs << tem->key << "->" << tem->left->key << "[label=left,color=blue]"
<< endl;
}
if (tem->right != tree->nil) {
ofs << tem->key << "->" << tem->right->key << "[label=right,color=red]"
<< endl;
}
if (tem->left != tree->nil)
q.push(tem->left);
if (tem->right != tree->nil)
q.push(tem->right);
}
ofs << "}" << endl;
ofs.close();
string cmd = "dot " + fileName + " -Tpng -o " + fileName + ".png";
system(cmd.c_str());
system(("rm -f " + fileName).c_str());
}

void LeftRotate(Tree *tree, TreeNode *x) {
TreeNode *y = x->right;
x->right = y->left;
if (y->left != tree->nil) {
y->left->p = x;
}
y->p = x->p;
if (x->p == tree->nil) {
tree->root = y;
} else if (x == x->p->left) {
x->p->left = y;
} else {
x->p->right = y;
}
y->left = x;
x->p = y;
}

void RightRotate(Tree *tree, TreeNode *y) {
cout << "right Rotate" << endl;
TreeNode *x = y->left;
y->left = x->right;
if (x->right != tree->nil) {
x->right->p = y;
}
x->p = y->p;
if (y->p == tree->nil) {
tree->root = x;
} else if (y == y->p->left) {
y->p->left = x;
} else {
y->p->right = x;
}
x->right = y;
y->p = x;
}

void RbTransplant(Tree *tree, TreeNode *oldNode, TreeNode *newNode) {
if (oldNode->p == tree->nil)
tree->root = newNode;
else if (oldNode == oldNode->p->left) {
oldNode->p->left = newNode;
} else {
oldNode->p->right = newNode;
}
newNode->p = oldNode->p;
}

void RbDeleteFixup(Tree *tree, TreeNode *x) {
while (x != tree->root && x->color == BLACK) {
if (x == x->p->left) { // x 是左子树
TreeNode *w = x->p->right;
if (w->color == RED) {
w->color = BLACK;
x->p->color = RED;
LeftRotate(tree, x->p);
w = x->p->right;
}
if (w->left->color == BLACK && w->right->color == BLACK) {
w->color = RED;
x = x->p;
} else if (w->right->color == BLACK) {
w->left->color = BLACK;
w->color = RED;
RightRotate(tree, w);
w = x->p->right;
} else {
w->color = x->p->color;
x->p->color = BLACK;
w->right->color = BLACK;
LeftRotate(tree, x->p);
x = tree->root;
}
} else { // x 是右子树
TreeNode *w = x->p->left;
if (w->color == RED) {
w->color = BLACK;
x->p->color = RED;
RightRotate(tree, x->p);
w = x->p->left;
}
if (w->right->color == BLACK && w->left->color == BLACK) {
w->color = RED;
x = x->p;
} else if (w->left->color == BLACK) {
w->right->color = BLACK;
w->color = RED;
LeftRotate(tree, w);
w = x->p->left;
} else {
w->color = x->p->color;
x->p->color = BLACK;
w->left->color = BLACK;
RightRotate(tree, x->p);
x = tree->root;
}
}
}
x->color = BLACK;
}

void RbDelete(Tree *tree, TreeNode *z) {
if ((tree == nullptr) || (z == nullptr) || (z == &gNilNode))
return;
TreeNode *y = z;
TreeNode *x;
RbColor yOriginalColor = y->color;
if (z->left == tree->nil) {
x = z->right;
RbTransplant(tree, z, z->right);
} else if (z->right == tree->nil) {
x = z->left;
RbTransplant(tree, z, z->left);
} else {
TreeNode *y = Minimun(z->right);
yOriginalColor = y->color;
x = y->right;
if (y->p == z) {
x->p = y;
} else {
RbTransplant(tree, y, y->right);
y->right = z->right;
y->right->p = y;
}
RbTransplant(tree, z, y);
y->left = z->left;
y->left->p = y;
y->color = z->color;
}
if (yOriginalColor == BLACK) {
RbDeleteFixup(tree, x);
}
}

void RbInsertFixup(Tree *tree, TreeNode *z) {
while (z->p->color == RED) {
if (z->p == z->p->p->left) {
TreeNode *y = z->p->p->right;
if (y->color == RED) {
z->p->color = BLACK;
y->color = BLACK;
z->p->p->color = RED;
z = z->p->p;
} else if (z == z->p->right) {
z = z->p;
LeftRotate(tree, z);
} else {
z->p->color = BLACK;
z->p->p->color = RED;
RightRotate(tree, z->p->p);
}
} else {
TreeNode *y = z->p->p->left;
if (y->color == RED) {
z->p->color = BLACK;
y->color = BLACK;
z->p->p->color = RED;
z = z->p->p;
} else if (z == z->p->left) {
z = z->p;
RightRotate(tree, z);
} else {
z->p->color = BLACK;
z->p->p->color = RED;
LeftRotate(tree, z->p->p);
}
}
}
tree->root->color = BLACK;
}

void RbInsert(Tree *tree, TreeNode *z) {
TreeNode *y = tree->nil;
TreeNode *iterTree = tree->root;
while (iterTree != tree->nil) {
y = iterTree;
if (z->key < iterTree->key)
iterTree = iterTree->left;
else
iterTree = iterTree->right;
}
z->p = y;
if (y == tree->nil) {
tree->root = z;
} else if (z->key < y->key) {
y->left = z;
} else {
y->right = z;
}
z->left = tree->nil;
z->right = tree->nil;
z->color = RED;
RbInsertFixup(tree, z);
}

TreeNode *AllocNode(int key) {
TreeNode *pNode = new TreeNode; //创建新结点
pNode->key = key;
pNode->p = NULL;
pNode->left = NULL;
pNode->right = NULL;
return pNode;
}

int main() {
gNilNode.color = BLACK;
Tree tree = {&gNilNode, &gNilNode};
int arr[] = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};

for (auto &key : arr) {
cout << "add key:" << key << endl;
TreeNode *pNode = AllocNode(key);
RbInsert(&tree, pNode);
produceGraphviz(&tree, to_string(key));
}
for (auto &key : arr) {
TreeNode *delNode = RbSearch(tree.root, key);
if (delNode != &gNilNode) {
cout << "del key:" << delNode->key << endl;
RbDelete(&tree, delNode);
delete delNode;
produceGraphviz(&tree, to_string(key) + "_del.dot");
}
}
tree.root = &gNilNode;
tree.nil = &gNilNode;
for (auto &key : arr) {
TreeNode *pNode = AllocNode(key);
RbInsert(&tree, pNode);
}
front(tree.root);
cout << endl;
middle(tree.root);
cout << endl;
back(tree.root);
cout << endl;
RbBackDelete(tree.root);
return 0;
}
