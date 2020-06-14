#include <fstream>
#include <iostream>
#include <queue>
using namespace std;

struct TreeNode {
  int key;
  struct TreeNode *p;
  struct TreeNode *lchild;
  struct TreeNode *rchild;
};
struct Tree {
  TreeNode *root;
};

void front(TreeNode *node) //前序遍历
{
  if (node) {
    cout << node->key << "  ";
    front(node->lchild);
    front(node->rchild);
  }
}

void middle(TreeNode *node) //中序遍历
{
  if (node) {
    middle(node->lchild);
    cout << node->key << "  ";
    middle(node->rchild);
  }
}

void back(TreeNode *node) //后序遍历
{
  if (node) {
    back(node->lchild);
    back(node->rchild);
    cout << node->key << "  ";
  }
}
void produceGraphviz(TreeNode *tree, const char *fileName) {
  if (!tree)
    return;
  std::ofstream ofs(fileName);
  ofs << "digraph  {" << endl;
  queue<TreeNode *> q;
  q.push(tree);
  while (!q.empty()) {
    TreeNode *tem = q.front();
    q.pop();
    if (tem->lchild != nullptr) {
      ofs << tem->key << "->" << tem->lchild->key << "[label=left,color=blue]"
          << endl;
    }
    if (tem->rchild != nullptr) {
      ofs << tem->key << "->" << tem->rchild->key << "[label=right,color=red]"
          << endl;
    }
    if (tem->lchild != NULL)
      q.push(tem->lchild);
    if (tem->rchild != NULL)
      q.push(tem->rchild);
  }
  ofs << "}" << endl;
  ofs.close();
}

void TreeInsert(Tree *tree, TreeNode *pNode) {
  TreeNode *destNode = nullptr;
  TreeNode *iterTree = tree->root;
  while (iterTree != nullptr) {
    destNode = iterTree;
    if (pNode->key < iterTree->key)
      iterTree = iterTree->lchild;
    else
      iterTree = iterTree->rchild;
  }
  pNode->p = destNode;
  if (destNode == nullptr) {
    tree->root = pNode;
  } else if (pNode->key < destNode->key) {
    destNode->lchild = pNode;
  } else {
    destNode->rchild = pNode;
  }
}

TreeNode *allocNode(int key) {
  TreeNode *pNode = new TreeNode; //创建新结点
  pNode->key = key;
  pNode->p = NULL;
  pNode->lchild = NULL;
  pNode->rchild = NULL;
  return pNode;
}

void Transplant(Tree *tree, TreeNode *pNode, TreeNode *pChNode) {
  if (pNode->p == nullptr)
    tree->root = pChNode;
  else if (pNode == pNode->p->lchild) {
    pNode->p->lchild = pChNode;
  } else {
    pNode->p->rchild = pChNode;
  }
  if (pChNode != nullptr) {
    pChNode->p = pNode->p;
  }
}
TreeNode *Minimun(TreeNode *pNode) {
  while (pNode->lchild != nullptr) {
    pNode = pNode->lchild;
  }
  return pNode;
}
void Delete(Tree *tree, TreeNode *pNode) {
  if ((tree == nullptr) || (pNode == nullptr))
    return;
  if (pNode->lchild == nullptr) {
    Transplant(tree, pNode, pNode->rchild);
  } else if (pNode->rchild == nullptr) {
    Transplant(tree, pNode, pNode->lchild);
  } else {
    TreeNode *y = Minimun(pNode->rchild);
    if (y->p != pNode) {
      Transplant(tree, y, y->rchild);
      y->rchild = pNode->rchild;
      y->rchild->p = y;
    }
    Transplant(tree, pNode, y);
    y->lchild = pNode->lchild;
    y->lchild->p = y;
  }
}
TreeNode *Search(TreeNode *pNode, int key) {
  if ((pNode == nullptr) || (pNode->key == key))
    return pNode;
  if (key < pNode->key)
    return Search(pNode->lchild, key);
  else
    return Search(pNode->rchild, key);
}

TreeNode *SearchLoop(TreeNode *pNode, int key) {
  while (pNode != nullptr && pNode->key != key) {
    if (key < pNode->key)
      pNode = pNode->lchild;
    else
      pNode = pNode->rchild;
  }
  return pNode;
}
void DeleteNode(TreeNode *node) {
  if (node) {
    DeleteNode(node->lchild);
    DeleteNode(node->rchild);
    delete node;
  }
}
int main() {
  Tree tree = {0};
  int arr[] = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};

  for (auto &key : arr) {
    TreeNode *pNode = allocNode(key);
    TreeInsert(&tree, pNode);
  }
  cout << "first order:" << endl;
  front(tree.root);
  cout << "\nmiddle order:" << endl;
  middle(tree.root);
  cout << "\nback order:" << endl;
  back(tree.root);
  cout << endl;
  produceGraphviz(tree.root, "src.dot");
  int key = 6;
  TreeNode *deleteNode = Search(tree.root, key);
  if (deleteNode != nullptr) {
    Delete(&tree, deleteNode);
    delete deleteNode;
  }
  produceGraphviz(tree.root, "src1.dot");
  DeleteNode(tree.root);
  return 0;
}
