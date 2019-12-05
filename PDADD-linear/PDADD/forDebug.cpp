#include "forDebug.h"
void showMatr(float **S, int N, int n)
{
	printf("---------------------\n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%f\t", S[i][j]);
		}
		printf("\n");
	}
	printf("---------------------\n");
}