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
