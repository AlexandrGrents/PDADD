#include "debug.h"
#include <iostream>
void print_matrix(float **matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			printf("%lf\t", matrix[i][j]);
		}
		printf("\n");
	}
	printf("------------------\n");
}
void print_array(float *arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%lf\n", arr[i]);
	}
	printf("++++++++++++++++++\n");
}
void print_two_array(float *arr1, float *arr2, int n)
{
	printf("\n******************\n");
	for (int i = 0; i < n; i++)
	{
		printf("%lf\t%lf\t%lf\n", arr1[i],arr2[i], (arr1[i] - arr2[i])*(arr1[i]-arr2[i]));
	}
	printf("******************\n");
}