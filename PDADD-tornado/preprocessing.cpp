#include <stdio.h>
#include <malloc.h>
#include <cmath>
void zNorm(float *t, float **S, int index, int n)
{
	double sum = 0;
	double sumSqr = 0;
	for (int i = index; i < index+n; i++)
	{
		sum += t[i];
		sumSqr += t[i] * t[i];
	}
	
	double u = sum / n;
	double q = sqrt(sumSqr / n - u*u);
	if (q < 0.001)
	{
		q = 0.001;
	}
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

