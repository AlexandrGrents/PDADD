/*
	Àëåêñàíäð Ãðåíö (2019 ã)
	Âåðñèÿ ïàðàëëåëüíîãî àëãîðèòìà ïîèñêà äèññîíàíñîâ âî âðåìåííîì ðÿäå äëÿ 1 óçëà ñ 1 ÿäðîì - ãîëîâíîé ìîäóëü
	Ïðåäíàçíà÷åí äëÿ âûïîëíåíèÿ âñåõ ñîñòàâíûõ ôàç àëãîðèòìà.

	Âõîäíûå äàííûå: òàáëèöà input.csv, ñîäåðæàùàÿ â ñåáå âðåìåííîé ðÿä
	Âûõîäíûå äàííûå: òàáëèöà output.csv, ñîäåðæàùàÿ â ñåáå èíäåêñû âñåõ äèàïàçîííûõ äèññîíàíñîâ, 
	òàáëèöà S.csv, ñîäåðæàùàÿ â ñåáå ìàòðèöó íîðìàëèçîâàííûõ ïîäïîñëåäîâàòåëüíîñòåé âðåìåííîãî ðÿäà èç èñõîäíîé òàáëèöû.
*/
#include "mpi.h"
#include <stdio.h>
#include <cstdlib> 
#include <iostream>

#include "preprocessing.h"
#include "refinement.h"
#include "selection.h"
#include "IOdata.h"
#include "ED.h"

#include "debug.h"

int main(int argc, char** argv)
{
	
	char inputFileName[] = "input0000.csv";
	MPI_Status stat;

	int m = 100000;
	int n = 128;
	float R = 0.08*n;
	int p = 4;
	int N;
	float dq = 0.6;
	

	const float r = R * R;
	//Ñòàðò ðàáîòû ïðîãðàììû

	printf("Start\n");

	int size, rank;
	int t_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//Âûäåëåíèå ïàìÿòè äëÿ âðåìåííîãî ðÿäà
	float* t;
	int m_local;
	//Ñ÷èòûâàíèå âðåìåííîãî ðÿäà è ïåðåäà÷à åãî íà óçëû
		
		
	if (rank == 0) m_local = (m - n + 1) / (size)+(n - 1) + (m - n + 1) % (size);
	else m_local = (m - n + 1) / (size)+(n - 1);
		
	inputFileName[5] = ('0' + rank / 1000);
	inputFileName[6] = ('0' + (rank / 100) % 10);
	inputFileName[7] = ('0' + (rank / 10) % 10);
	inputFileName[8] = ('0' + rank % 10);

	FILE* file = fopen(inputFileName, "r");
	
	t = (float*)calloc(m_local, sizeof(float));

	for (int i = 0; i < m_local; i++)
	{
		fscanf(file, "%f", &t[i]);
	}

	fclose(file);



	N = m_local - n + 1;
	int L = (int)(dq * N + 1);
	
	
	
	
	//Âûäåëåíèå ïàìÿòè äëÿ ìàòðèöû ïîäïîñëåäîâàòåëüíîñòåé
	float **S;
	S = (float**)calloc(N, sizeof(float*));
	for (int i = 0; i < N; i++)
	{
		S[i] = (float*)calloc(n, sizeof(float));
	}

	//Çàïîëíåíèå ìàòðèöû ïîäïîñëåäîâàòåëüíîñòåé (êàæäàÿ ïîäïîñëåäîâàòåëüíîñòü íîðìàëèçóåòñÿ)
	fill_S(t, S, n, N);
	
	//print_matrix(S, N, n);

	//Âûäåëåíèå ïàìÿòè äëÿ ñïèñêà êàíäèäàòîâ è ìàòðèöû êàíäèäàòîâ
	
	
	
	//Ôàçà ïîèñêà
	

	

	int add = 0;
	if (rank == 0)
	{
		add = m - m_local;
	}
	else
	{
		add = (m_local - n + 1) * (rank - 1);
	}

	//printf("add = %d\n", add);


	int* Insert = (int*)calloc(p, sizeof(int));	int* Bottom = (int*)calloc(p, sizeof(int));	int* Count = (int*)calloc(p, sizeof(int));
	for (int i = 0; i < p; i++)	{Insert[i] = 0;	Bottom[i] = 1;Count[i] = 0;	}

	int** I = (int**)calloc(p, sizeof(int*));
	for (int i = 0; i < p; i++)	{I[i] = (int*)calloc(L, sizeof(int));for (int j = 0; j < L; j++){I[i][j] = NIL;}}


	findCandidats(S, r, N, n, L, p, I, Insert, Bottom, Count);

	int H = 0;
	


	for (int k = 0; k < p; k++)
	{
		H += Count[k];
	}

	int* sizes = (int*)calloc(size, sizeof(int));
	sizes[rank] = H;
	for (int i = 0; i < size; i++)
	{
		if (i == rank)
		{
			for (int j = 0; j < size; j++)
			{
				if (j != rank) MPI_Send(&H, 1, MPI_INT, j, 2, MPI_COMM_WORLD);
			}
		}
		else
		{
			MPI_Recv(sizes + i, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &stat);
		}
	}

	//for (int i = 0; i < size; i++) printf("%d ", sizes[i]); printf("\n");

	int sum = 0, pre = 0;
	for (int i = 0; i < size; i++)
	{
		sum += sizes[i];
		if (i < rank) pre += sizes[i];
	}

	int* Candidats = (int*)calloc(sum, sizeof(int));
	float* C = (float*)calloc((sum*n), sizeof(float));

	int i = pre;
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

	//for (int i = 0; i < H; i++) printf("%d ", Candidats[pre+i]); printf("\n");

	//printf("pre = %d\n", pre);
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C[(pre+i)*n+j]= S[Candidats[pre+i]][j];
			//printf("%f [%d] -> %f [%d,%d]\n", C[(pre + i) * n + j], (pre + i) * n + j, S[Candidats[i]][j], Candidats[i],j);
		}
		Candidats[i] += add;
	}
	/*printf("+++++++++++++++\n");
	for (int i = 0; i < sum; i++)
	{
		printf("[%d]\t", Candidats[i]);
		for (int j = 0; j < n; j++)
		{
			printf("%f\t", C[i * n + j]);
		}
		printf("\n");
	}*/

	int pre_now = 0;

	for (int i = 0; i < size; i++)
	{
		if (i == rank)
		{
			for (int j = 0; j < size; j++)
			{
				if (j != rank)
				{
					MPI_Send(Candidats + pre, H, MPI_INT, j, 3, MPI_COMM_WORLD);
					MPI_Send(C + (pre * n), H * n, MPI_FLOAT, j, 4, MPI_COMM_WORLD);
				}
			}
		}
		else
		{
			MPI_Recv(Candidats+ pre_now, sizes[i], MPI_INT, i, 3, MPI_COMM_WORLD, &stat);
			MPI_Recv(C + (pre_now * n), sizes[i] * n, MPI_FLOAT, i, 4, MPI_COMM_WORLD, &stat);
		}
		pre_now += sizes[i];
	}

	//for (int i = 0; i < sum; i++) printf("%d ", Candidats[i]); printf("\n");




	//for (int i = 0; i < sum; i++)
	//{
	//	printf("[%d]\t", Candidats[i]);
	//	for (int j = 0; j < n; j++)
	//	{
	//		printf("%f\t", C[i * n + j]);
	//	}
	//	printf("\n");
	//}







	//Âûäåëåíèå ïàìÿòè äëÿ ñïèñêà äèññîíàíñîâ
	int D;
	int *Discords = (int*)calloc(H, sizeof(int));

	
	int sum_H = sum;


	//Ôàçà óòî÷íåíèÿ
	refinement(S, C, Candidats, Discords, r, N, n, sum_H, &D, p,add);


	for (int i = 0; i < D; i++) printf("%d ", Discords[i]); printf("\n");

	sizes[rank] = D;
	for (int i = 0; i < size; i++)
	{
		if (i == rank)
		{
			for (int j = 0; j < size; j++)
			{
				if (j != rank) MPI_Send(&D, 1, MPI_INT, j, 2, MPI_COMM_WORLD);
			}
		}
		else
		{
			MPI_Recv(sizes + i, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &stat);
		}
	}
	
	sum = 0;
	pre = 0;
	for (int i = 0; i < size; i++)
	{
		sum += sizes[i];
		if (i < rank) pre += sizes[i];
	}

	int max = 0, tmp;
	bool fl;
	for (int i = 0; i < size; i++) if (max < sizes[i]) max = sizes[i];

	int* other_discords = (int*)calloc(max, sizeof(int));

	if (rank != 0) MPI_Send(Discords, D, MPI_INT, 0, 7, MPI_COMM_WORLD);
	else
	{
		for (int k = 1; k < size; k++)
		{
			MPI_Recv(other_discords, sizes[k], MPI_INT, k, 7, MPI_COMM_WORLD, &stat);
			i = 0;
			while (i<D)
			{
				fl = true;
				for (int j = 0; j < sizes[k]; j++)
				{
					if (Discords[i] == other_discords[j])
					{
						fl = false;
						break;
					}
				}
				if (fl)
				{
					printf("--> %d\n", Discords[i]);
					Discords[i] = Discords[D - 1];
					D--;
					i--;
				}
				i++;
			}
		}
	}

	//Âûâîä ðåçóëüòàòîâ íà ýêðàí è â ôàéë
	if (rank == 0)
	{
		printf("Find %d candidats\n", sum_H);
		printf("Find %d discords\n", D);
		writeDiscords("output.txt", D, Discords);
		//Îêîí÷àíèå ðàáîòû ïðîãðàììû
		
	}


	MPI_Finalize();
	return 0;
}        