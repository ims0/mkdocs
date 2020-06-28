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

int *ComputePrefixFunc(char *P, int M) {
  int *stepIndex = new int[M];
  stepIndex[0] = 0;
  int k = 0;

  for (int q = 1; q < M; ++q) {
    while (k > 0 && (P[k] != P[q])) {
      k = stepIndex[k];
    }
    if (P[k] == P[q]) {
      ++k;
    }
    stepIndex[q] = k;
  }
  return stepIndex;
}

template <size_t N, size_t M> void KmpMatcher(char (&T)[N], char (&P)[M]) {

  print(P);
  int *stepIndex = ComputePrefixFunc(P, M);
  print(stepIndex, M);

  int q = 0;
  for (size_t i = 0; i < N; ++i) {
    while (q > 0 && P[q] != T[i]) {
      q = stepIndex[q];
    }
    if (P[q] == T[i]) {
      ++q;
    }
    if (q == M) {
      cout << "match at:" << i - M << endl;
      q = stepIndex[M - 1];
    }
  }
  delete[] stepIndex;
}

int main() {

  char T[] = "bacb_ababacab_ababacab_abab";
#define PATTERN ("ababacab")
  char p[sizeof(PATTERN) - 1];
  const char *P = PATTERN;
  sprintf(p, P, sizeof(PATTERN) - 1);

  KmpMatcher(T, p);
#define PATTERN2 ("abab")
  char pp[sizeof(PATTERN2) - 1];
  const char *PP = PATTERN;
  sprintf(pp, PP, sizeof(PATTERN) - 1);

  KmpMatcher(T, pp);
  return 0;
}
