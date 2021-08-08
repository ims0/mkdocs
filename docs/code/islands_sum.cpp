/*********************************************************
  > File Name: islands_sum.cpp
  > Author: ims
  > Created Time: Tue 03 Aug 2021 11:32:05 AM CST
 *********************************************************/

#include <iostream>
using namespace std;

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
            if( matrix[i][j] == 1 )
            {
                sum++;
                check(i,j);
            }
        }
    }
    printf("sum:%d\n", sum);
    return 0; 
}
