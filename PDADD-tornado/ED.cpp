#include "ED.h"
#include <cmath>
double ED_sqr(float *firstSubsequence, float *secondSubsequence, int n)
{
	int k=0;
	double ED_sqr = 0;
	for (k =0; k<n/128;k++)
	{
	for (int i = 0; i < 128; i++)
		{
			ED_sqr += (firstSubsequence[128*k+i] - secondSubsequence[128*k+i])*(firstSubsequence[128*k+i] - secondSubsequence[128*k+i]);
			// ED_sqr += pow(firstSubsequence[i] - secondSubsequence[i],2);
		}
	}
	k=n/128;
	for (int i =0; i<n%128;i++)
	{
		ED_sqr += (firstSubsequence[128*k+i] - secondSubsequence[128*k+i])*(firstSubsequence[128*k+i] - secondSubsequence[128*k+i]);
	}
	return ED_sqr;
}
double EarlyED_sqr(float *firstSubsequence, float *secondSubsequence, int n, float r)
{
	double ED_sqr = 0;
	for (int i=0; i<n; i++)
	{
		ED_sqr +=  pow(firstSubsequence[i] - secondSubsequence[i],2);
		if (ED_sqr>r) break;
	}
	return ED_sqr;
}

double ED_norm(float *first, float *second, int n, float u1, float u2, float q1, float q2)
{
	double q_sum = 0;
	for (int i =0; i<n; i++)
	{
		q_sum += first[i]*second[i];
	}
	return 2*n*(1-(q_sum - n*u1*u2)/(n*q1*q2));
}