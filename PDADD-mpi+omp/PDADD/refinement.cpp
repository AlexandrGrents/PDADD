#include <malloc.h>
#include "ED.h"
#include "omp.h"
#include <iostream>
void findDiscords(float** S, float* C, int* Candidats, float r, int N, int n, int H, int p, bool** B, int add)
{
	//начало параллельной области
#pragma omp parallel num_threads(p)
	{
		
		int iam = omp_get_thread_num();
#pragma omp critical
		{
			printf("Create thread #%d\n", iam);
		}
#pragma omp for
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < H; j++)
			{

				if (Candidats[j] != i + add)
				{

					if (B[iam][j])
					{
						B[iam][j] = ED_sqr(S[i], C + j * n, n) > r;
					}
				}

			}
		}
		
	}
	//конец параллельной области
}
void getDiscords(bool ** B, int *Candidats, int *Discords, int H, int *D, int p)
{
	*D = 0;
	bool isDiscord;
	for (int j = 0; j < H; j++)
	{
		isDiscord = true;
		for (int k = 0; k < p; k++)
		{
			isDiscord = isDiscord && B[k][j];
		}
		if (isDiscord)
		{
			Discords[*D] = Candidats[j];
			*D = (*D) + 1;
		}
	}
	/*Discords = (int*)realloc(Discords, (*D)*sizeof(int));*/
}
void refinement(float **S, float *C, int *Candidats, int *Discords, float r, int N, int n, int H, int *D, int p,int add)
{
	bool **B = (bool**)calloc(p, sizeof(bool*));
	for (int i = 0; i < p; i++) 
	{ 
		B[i] = (bool*)calloc(H, sizeof(bool)); 
		for (int j = 0; j < H; j++)
		{
			B[i][j] = true;
		}
	}
	findDiscords(S, C, Candidats, r, N, n, H, p, B,add);

	getDiscords(B, Candidats, Discords, H, D, p);
}