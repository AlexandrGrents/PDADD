#include <stdio.h>
#include <cstdlib> 
#include <iostream>
#include "config.h"
#include "preprocessing.h"
#include "refinement.h"
#include "selection.h"
#include "IOdata.h"
#include "ED.h"
#include "forDebug.h"

int main()
{
	float *t = (float*)calloc(m, sizeof(float));
	
	getTimeSeries("input.csv", m, t);
	
	zNorm(t, m);
	
	float **S;

	S = (float**)calloc(N, sizeof(float*));
	for (int i = 0; i < N; i++)
	{
		S[i] = (float*)calloc(n, sizeof(float));
	}

	fill_S(t, S, n, N);
	write_S("S.csv", N, n, S);
	//showMatr(S, N, n);

	int H;
	int *Candidats = (int*)calloc(L,sizeof(int));
	float **C = (float**)calloc(L, sizeof(float*));
	
	selection(S, C, r, N, n, L, p, &H, Candidats);

	//showMatr(C, H, n);

	

	int D;
	int *Discords = (int*)calloc(H, sizeof(int));

	refinement(S, C, Candidats, Discords, r, N, n, H, &D, p);

	printf("D = %d\n", D);
	
	writeDiscords("output.csv", D, Discords);

	system("pause");
	return 0;
}