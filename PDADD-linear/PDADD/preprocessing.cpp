#include <stdio.h>
#include <malloc.h>
#include <cmath>
void zNorm(float *t, float **S, int index, int n)
{
	float sum = 0;
	float sumSqr = 0;
	for (int i = index; i < index+n; i++)
	{
		sum += t[i];
		sumSqr += t[i] * t[i];
	}
	
	float u = sum / n;
	float q = sqrt(sumSqr / n - u*u);
	for (int j=0; j<n; j++)
	{
		S[index][j] = (t[index + j] - u) / q;
	}
}
void fill_S(float *t, float **S, int n, int N)
{
	for (int i = 0; i < N; i++)
	{
		zNorm(t, S, i, n);
		
	}
}
