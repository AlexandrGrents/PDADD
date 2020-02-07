#include "ED.h"
float ED_sqr(float *firstSubsequence, float *secondSubsequence, int n)
{
	float ED_sqr = 0;
	for (int i = 0; i < n; i++)
	{
		ED_sqr += pow(firstSubsequence[i] - secondSubsequence[i], 2);
	}
	return ED_sqr;
}