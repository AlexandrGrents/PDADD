#include <malloc.h>
#include "ED.h"
#include "config.h"
#include <stdio.h>

void findCandidats(	float **S, 	float r, int N, int n, int L, int p, int **I, int *Insert, int *Bottom,	int *Count)
{
	bool isCandidate;
	//����� ������������ �������
	int iam = 0;
	for (int i = 0; i < N; i++)
	{
		isCandidate = true;
		for (int j = 0; j < Bottom[iam]; j++)
		{
			if (I[iam][j] != NIL)
			{
				if (i - I[iam][j] >= n)
				{
					if (ED_sqr(S[i], S[I[iam][j]], n) < r)
					{
						isCandidate = false;
						//printf("--%d\n", I[iam][j]);
						I[iam][j] = NIL;
						Insert[iam] = j;
						Count[iam]--;
					}
				}
				
			}
		}
		if (isCandidate)
		{
			//printf("++%d\n", i);
			if (I[iam][Insert[iam]] == NIL)
			{
				I[iam][Insert[iam]] = i;
			}
			else
			{
				I[iam][Bottom[iam]] = i;
				Bottom[iam]++;
			}
			Count[iam]++;
		}
	}
	//����� ������������ �������






}

void getCandidats(int **I, int *Candidats, int *Bottom, int *Count, int L, int p, int *H)
{
	*H = 0;
	int i = 0;
	for (int k = 0; k < p; k++)
	{
		*H += Count[k];
	}
	//Candidats = (int*)realloc(Candidats, (*H)*sizeof(int));
	for (int k = 0; k < p; k++)
	{
		for (int j = 0; j < Bottom[k]; j++)
		{
			if (I[k][j] != NIL)
			{
				Candidats[i] = I[k][j];
				i++;
			}
		}
	}

}

void get_C(float **S, float **C, int *Candidats, int *H, int n)
{
	
	for (int i = 0; i < *H; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C[i][j] = S[Candidats[i]][j];
		}
	}
}

void selection(float **S, float **C, float r, int N, int n, int L, int p, int *H, int *Candidats)
{
	int *Insert = (int*)calloc(p, sizeof(int));
	int *Bottom = (int*)calloc(p, sizeof(int));
	int *Count = (int*)calloc(p, sizeof(int));
	for (int i = 0; i < p; i++)
	{
		Insert[i] = 0;
		Bottom[i] = 1;
		Count[i] = 0;
	}
	int **I = (int**)calloc(p, sizeof(int*));
	for (int i = 0; i < p; i++)
	{
		I[i] = (int*)calloc(L, sizeof(int));
		for (int j = 0; j < L; j++)
		{
			I[i][j] = NIL;
		}
	}
		
	findCandidats(S, r, N, n, L, p, I, Insert, Bottom, Count);

	printf("Bottom = %d, L = %d\n", Bottom[0], L);
	for (int i = 0; i < Bottom[0]; i++)
	{
		printf("I[0][%d] = %d\n", i, I[0][i]);
	}

	getCandidats(I, Candidats, Bottom, Count, L, p, H);

	printf("H = %d\n", *H);
	for (int i = 0; i < *H; i++)
	{
		printf("Candidat[%d] = %d\n", i, Candidats[i]);
	}

	C = (float**)realloc(C, (*H)*sizeof(float*));
	for (int i = 0; i < (*H); i++) 
	{ 
		printf("-->%d\n", i);
		if (i<*H)
		C[i] = (float*)calloc(n, sizeof(float));
	}

	get_C(S, C, Candidats, H, n);
}