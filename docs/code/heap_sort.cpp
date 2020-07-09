
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
static void heap_arrange(int arr[], int cur, int size);
 
static int heap_verify(int arr[], int size)
{
	int i;
	for (i = 0; i < size / 2; ++i) {
		int lhs = 2 * i + 1;
		int rhs = 2 * i + 2;
 
		if (lhs < size && arr[i] > arr[lhs]) {
			fprintf(stderr, "arr[%d]:%d > arr[%d]:%d\n", i, arr[i], lhs, arr[lhs]);
			return -1;
		}
		if (rhs < size && arr[i] > arr[rhs]) {
			fprintf(stderr, "arr[%d]:%d > arr[%d]:%d\n", i, arr[i], rhs, arr[rhs]);
			return -1;
		}
	}
	return 0;
}
 
static void heap_print(int arr[], int size)
{
	int layer = 0, num = 0;
	for (layer = 0; num < size; ++layer) {
		int i = 0;
		for (i = 0 ; i < (1 << layer) && num < size ; ++i)
			printf("%3d ", arr[num++]);
		printf("\n");
	}
}
 
//  调整为小顶堆
static void heap_arrange(int arr[], int cur, int size)  //调整为小顶堆
{
 
	while (cur < size) {
		int left = 2 * cur + 1;
		int right = 2 * cur + 2;
		int min = -1;
		int min_val = arr[cur];
		if (left < size && arr[left] < min_val) { //检查是否比左节点大
			min = left;
			min_val = arr[left];
		}
		if (right < size && arr[right] < min_val) {//检查是否比右节点大
			min = right;
			min_val = arr[right];
		}
		if (min == -1)
		{
			printf("当前点%d，左子树：%d,右子树%d.合理退出\n"  ,arr[cur],arr[left] ,arr[right]);
			break;
		}
		int heaptop_val = arr[cur]; //堆顶的值
		arr[cur] =min_val;
		arr[min] = heaptop_val;
		cur = min;
		printf("交换%d\n",min_val);
		heap_print(arr, size);
	}
}
 
static void heap_sort(int arr[], int size)
{
	int i;
 
 
	for (i = size / 2-1 ; i >= 0; --i) {
		heap_arrange(arr, i, size);	//  建堆
	}
	printf("make heap:%d\n", i);
	heap_print(arr, size);
	assert(heap_verify(arr, size) == 0);
 
	for (i = size - 1; i > 0; --i) {
		int tmp;
		tmp = arr[0];
		arr[0] = arr[i];
		arr[i] = tmp;   //arr[i]<-->arr[size-1]
 
		printf("sort i=%d\n", i);
		heap_print(arr, size);
		heap_arrange(arr, 0, i);//建堆
		heap_print(arr, size);
		assert(heap_verify(arr, i) == 0);
	}
	printf("sorted:\n");
	heap_print(arr, size);
}
static int input(int **arr, int *size)
{
	int i;
	int ret;
	printf("输入变量个数:\n");
 
	ret = fscanf(stdin,  "%d", size);
	//if (ret != 1)
	//	return -1;
 
	printf("输入变量:\n");
	*arr = (int *)malloc(sizeof(int) * (*size));
	for (i = 0; i < *size; ++i) {
		fscanf(stdin, "%d", &(*arr)[i]);
	}
	return 0;
}
int main(int argc, char *argv[])
{
	int *arr = NULL;
	int size = 0;
	int i;
 
	if (input(&arr, &size) < 0) {
		fprintf(stderr, "input error\n");
		return 0;
	}
	printf("origin:\n");
	heap_print(arr, size);
 
	heap_sort(arr, size);
	return 0;
}