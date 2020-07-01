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

void fill_u_and_q(float *t, float **u_and_q, int n, int N)
{
	for (int i=0; i<N; i++)
	{
		u_and_q[0][i] = 0;
		u_and_q[1][i] = 0; 
		for (int j = i; j<i+n; j++)
		{
			u_and_q[0][i] += t[j];
			u_and_q[1][i] += t[j]*t[j];
		}
		u_and_q[0][i] = u_and_q[0][i]/n;
		u_and_q[1][i] = u_and_q[1][i]/n - u_and_q[0][i] * u_and_q[0][i];
	}
}