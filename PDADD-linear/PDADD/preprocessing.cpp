#include <stdio.h>
#include <malloc.h>
float get_u(float*t, int m)
{
	float sum = 0;
	for (int i = 0; i < m; i++)
	{
		sum += t[i];
	}
	return sum / m;
}
float get_q(float *t, int m, float u)
{
	float sumSqr = 0;
	for (int i = 0; i < m; i++)
	{
		sumSqr += t[i] * t[i];
	}
	return sumSqr / m - u*u;
}
void zNorm(float *t, int m)
{
	float u = get_u(t, m);
	float q = get_q(t, m, u);
	printf("u = %f\nq = %f\n", u, q);
	for (int i = 0; i < m; i++)
	{
		t[i] = (t[i]-u)/q;
	}
}
void fill_S(float *t, float **S, int n, int N)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < n; j++)
		{
			S[i][j] = t[i + j];
		}
	}
}
