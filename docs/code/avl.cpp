#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

typedef int Comparable;
struct AvlNode {
  Comparable element;
  AvlNode *left;
  AvlNode *right;
  int height;

  AvlNode(const Comparable &ele, AvlNode *lt, AvlNode *rt, int h = 0)
      : element{ele}, left{lt}, right{rt}, height{h} {}

  AvlNode(Comparable &&ele, AvlNode *lt, AvlNode *rt, int h = 0)
      : element{std::move(ele)}, left{lt}, right{rt}, height{h} {}
};

/**
 * Return the height of node t or -1 if nullptr.
 */
int height(AvlNode *t) { return t == nullptr ? -1 : t->height; }

/**
 * Rotate binary tree node with left child.
 * For AVL trees, this is a single rotation for case 1.
 * Update heights, then set new root.
 */
void rotateWithLeftChild(AvlNode *&k2) {
  AvlNode *k1 = k2->left;
  k2->left = k1->right;
  k1->right = k2;
  k2->height = max(height(k2->left), height(k2->right)) + 1;
  k1->height = max(height(k1->left), k2->height) + 1;
  k2 = k1;
}
/**
 * Rotate binary tree node with left child.
 * For AVL trees, this is a single rotation for case 1.
 * Update heights, then set new root.
 */
void rotateWithRightChild(AvlNode *&k1) {
  AvlNode *k2 = k1->right;
  k1->right = k2->left;
  k2->left = k1;
  k1->height = max(height(k1->left), height(k1->right)) + 1;
  k2->height = max(height(k2->right), k1->height) + 1;
  k1 = k2;
}

/**
 * Double rotate binary tree node: first left child
 * with its right child; then node k3 with new left child.
 * For AVL trees, this is a double rotation for case 2.
 * Update heights, then set new root.
 */
void doubleWithLeftChild(AvlNode *&k3) {
  rotateWithRightChild(k3->left);
  rotateWithLeftChild(k3);
}

void doubleWithRightChild(AvlNode *&k3) {
  rotateWithLeftChild(k3->right);
  rotateWithRightChild(k3);
}

static const int ALLOWED_IMBALANCE = 1;

// Assume t is balanced or within one of being balanced
void balance(AvlNode *&t) {
  if (t == nullptr)
    return;

  if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
    if (height(t->left->left) >= height(t->left->right))
      rotateWithLeftChild(t);
    else
      doubleWithLeftChild(t);
  else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
    if (height(t->right->right) >= height(t->right->left))
      rotateWithRightChild(t);
    else
      doubleWithRightChild(t);

  t->height = max(height(t->left), height(t->right)) + 1;
}
/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
void insert(const Comparable &x, AvlNode *&t) {
  if (t == nullptr)
    t = new AvlNode{x, nullptr, nullptr};
  else if (x < t->element)
    insert(x, t->left);
  else if (t->element < x)
    insert(x, t->right);

  balance(t);
}

AvlNode *findMin(AvlNode *&t) {
  while (t->left != nullptr) {
    t = t->left;
  }
  return t;
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
void remove(const Comparable &x, AvlNode *&t) {
  if (t == nullptr)
    return; // Item not found; do nothing

  if (x < t->element)
    remove(x, t->left);
  else if (t->element < x)
    remove(x, t->right);
  else if (t->left != nullptr && t->right != nullptr) // Two children
  {
    t->element = findMin(t->right)->element;
    remove(t->element, t->right);
  } else {
    AvlNode *oldNode = t;
    t = (t->left != nullptr) ? t->left : t->right;
    delete oldNode;
  }

  balance(t);
}
void produceGraphviz(AvlNode *&tree, const string fileName) {
  if (tree == nullptr)
    return;
  std::ofstream ofs(fileName);
  ofs << "digraph {" << endl;
  queue<AvlNode *> q;
  q.push(tree);
  while (!q.empty()) {
    AvlNode *tem = q.front();
    q.pop();
    if (tem->left != nullptr) {
      ofs << tem->element << "->" << tem->left->element
          << "[label=left,color=blue]" << endl;
    }
    if (tem->right != nullptr) {
      ofs << tem->element << "->" << tem->right->element
          << "[label=right,color=red]" << endl;
    }
    if (tem->left != nullptr)
      q.push(tem->left);
    if (tem->right != nullptr)
      q.push(tem->right);
  }
  ofs << "}" << endl;
  ofs.close();
  string cmd = "dot " + fileName + " -Tpng -o " + fileName + ".png";
  system(cmd.c_str());
  system(("rm -f " + fileName).c_str());
}

int main() {
  srand(time(NULL));
  AvlNode *p = nullptr;
  for (int i = 0; i < 20; ++i) {
    int key = rand()&0X3F;
    cout << "add key:" << key << endl;
    insert(key, p);
  }

  cout << "end" << endl;
  produceGraphviz(p, "AvlTree");
}
