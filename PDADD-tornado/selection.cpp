#include "selection.h"
#include "omp.h"
void findCandidats(	float **S, 	float r, int N, int n, int L, int p, int **I, int *Insert, int *Bottom,	int *Count)
{
	int NIL = -1;
#pragma omp parallel num_threads(p)
	{
		//äàëåå ïàðàëëåëüíàÿ îáëàñòü
		bool isCandidate;
		int iam = omp_get_thread_num();
#pragma omp for
		for (int i = 0; i < N; i++)
		{
			//if (i%6000 == 0) printf("%d\n", i);
			isCandidate = true;
			for (int j = 0; j < Bottom[iam]; j++)
			{
				if (I[iam][j] != NIL)
				{
					if (i - I[iam][j] >= n)
					{
						if (ED_sqr(S[i], S[I[iam][j]], n) < r)
						// if (ED_norm(S[i], S[I[iam][j]], n,0,0,1,1) < r)
						{
							isCandidate = false;

							I[iam][j] = NIL;
							Insert[iam] = j;
							Count[iam]--;

						}

					}

				}
			}
			if (isCandidate)
			{
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
		//êîíåö ïàðàëëåëüíîé îáëàñòè
	}
}

void findCandidats_norm(float *t, float **u_and_q,	float r, int N, int n, int L, int p, int **I, int *Insert, int *Bottom,	int *Count)
{
	int NIL = -1;
#pragma omp parallel num_threads(p)
	{
		//äàëåå ïàðàëëåëüíàÿ îáëàñòü
		bool isCandidate;
		int iam = omp_get_thread_num();
#pragma omp for
		for (int i = 0; i < N; i++)
		{
			//if (i%6000 == 0) printf("%d\n", i);
			isCandidate = true;
			for (int j = 0; j < Bottom[iam]; j++)
			{
				if (I[iam][j] != NIL)
				{
					if (i - I[iam][j] >= n)
					{
						// if (ED_sqr(S[i], S[I[iam][j]], n) < r)
						if (ED_norm(t+i, t+I[iam][j], n, u_and_q[0][i], u_and_q[0][I[iam][j]], 
							u_and_q[1][i],u_and_q[1][I[iam][j]]) < r)
						{
							isCandidate = false;

							I[iam][j] = NIL;
							Insert[iam] = j;
							Count[iam]--;

						}

					}

				}
			}
			if (isCandidate)
			{
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
		//êîíåö ïàðàëëåëüíîé îáëàñòè
	}
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
void get_C_norm(float *t, float **u_and_q, float **C, int *Candidats, int *H, int n)
{
	for (int i=0; i<*H; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C[i][j] = (t[Candidats[i] + j] - u_and_q[0][Candidats[i]])/u_and_q[1][Candidats[i]];
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

	getCandidats(I, Candidats, Bottom, Count, L, p, H);
	
	

	//C = (float**)realloc(C, (*H)*sizeof(float*));
	for (int i = 0; i < (*H); i++) 
	{ 
		C[i] = (float*)calloc(n, sizeof(float));
	}

	get_C(S, C, Candidats, H, n);
}
