/*********************************************************
    > File Name: kmpMatcher.cpp
    > Author: ims
    > Created Time: 2020年06月28日 星期日 22时32分05秒
 *********************************************************/

#include <iostream>
#include <string.h>
using namespace std;

template <typename Type, int N> void print(Type (&A)[N]) {
  for (int i = 0; i < N; i++) {
    cout << A[i];
  }
  cout << endl;
}

int *ComputePrefixFunc(char *P, int M) {
  cout << "M:" << M << endl;
  for (int i = 0; i < M; i++) {
    cout << P[i];
  }
  cout << endl;
  int *stepIndex = new int[M];
  memset(stepIndex, 0, M);
  int k = 0;

  for (int q = 1; q < M; ++q) {
    while (k > 0 && (P[k] != P[q])) {
      k = stepIndex[k];
    }
    if (P[k] == P[q]) {
      k = k + 1;
    }
    stepIndex[q] = k;
  }
  return stepIndex;
}

template <size_t N, size_t MM> void KmpMatcher(char (&T)[N], char (&P)[MM]) {
  int M = MM - 1;
  int *stepIndex = ComputePrefixFunc(P, M);
  for (int i = 0; i < M; i++) {
    cout << stepIndex[i];
  }
  cout << endl;

  int q = 0;
  for (size_t i = 0; i < N; ++i) {
    while (q > 0 && P[q] != T[i]) {
      q = stepIndex[q];
    }
    if (P[q] == T[i]) {
      q = q + 1;
    }
    if (q == M) {
      cout << "match at:" << i - M << endl;
      q = stepIndex[q];
    }
  }
  delete[] stepIndex;
}

int main() {

  char T[] = "bacb_ababaca_ababaca";
  char P[] = "ababaca";
  print(P);

  KmpMatcher(T, P);

  return 0;
}
