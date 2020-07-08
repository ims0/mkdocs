/*********************************************************
    > File Name: kmpMatcher.cpp
    > Author: ims
    > Created Time: 2020年06月28日 星期日 22时32分05秒
 *********************************************************/

#include <iostream>
using namespace std;

template <typename Type, int N> void print(Type (&A)[N]) {
  for (int i = 0; i < N; i++) {
    cout << A[i];
  }
  cout << endl;
}
template <typename Type> void print(Type &A, int N) {
  for (int i = 0; i < N; i++) {
    cout << A[i];
  }
  cout << endl;
}

int *ComputePrefixFunc(const char *P, int M) {
  int *next = new int[M];
  next[0] = 0;
  int k = 0;

  for (int q = 1; q < M; ++q) {
    while (k > 0 && (P[k] != P[q])) {
      k = next[k - 1];
    }
    if (P[k] == P[q]) {
      ++k;
    }
    next[q] = k;
  }
  return next;
}

template <size_t N, size_t MM>
void KmpMatcher(char (&T)[N], const char (&P)[MM]) {
  size_t M = MM - 1;
  print(P);
  int *next = ComputePrefixFunc(P, M);
  print(next, M);

  size_t q = 0;
  for (size_t i = 0; i < N; ++i) {
    while (q > 0 && P[q] != T[i]) {
      q = next[q - 1];
    }
    if (P[q] == T[i]) {
      ++q;
    }
    if (q == M) {
      cout << "match at:" << i - M << endl;
      q = next[q - 1];
    }
  }
  delete[] next;
}

int main() {

  char T[] = "bacb_ababacab_ababacab_abab";

  cout << "txt:" << T << endl;
  KmpMatcher(T, "ababaaab");
  cout << "---" << endl;
  KmpMatcher(T, "ababacab");
  cout << "---" << endl;
  KmpMatcher(T, "abab");
  return 0;
}
