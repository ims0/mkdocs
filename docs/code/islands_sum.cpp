/*********************************************************
  > File Name: islands_sum.cpp
  > Author: ims
  > Created Time: Tue 03 Aug 2021 11:32:05 AM CST
 *********************************************************/

#include <iostream>
using namespace std;

int max_isoland=0;
int single=0;
int matrix[][4] = 
{{1, 1, 1, 1},
    {1, 1, 1, 1}, 
    {0, 0, 0, 0}, 
    {1, 1, 1, 1}
};

void check(int i, int j){
    if(i<0 || j<0 || j>3 || i>3 || matrix[i][j] == 0)
    {
        return ;
    }
    matrix[i][j] = 0;
    single++;
    check(i-1,j);
    check(i+1,j);
    check(i,j+1);
    check(i,j-1);
}
int main() { 
    int sum = 0;
    for( int i = 0 ; i<4 ; i++ )
    {
        for( int j=0 ; j< 4 ; ++j )
        {
            single=0;
            if( matrix[i][j] == 1 )
            {
                sum++;
                check(i,j);
            }
            if( single>max_isoland )
            {
                max_isoland = single;
            }
        }
    }
    printf("sum:%d\n", sum);
    printf("num:%d\n", max_isoland);
    return 0; 
}
