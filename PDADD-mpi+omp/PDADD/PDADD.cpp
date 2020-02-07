/*
	Александр Гренц (2019 г)
	Версия параллельного алгоритма поиска диссонансов во временном ряде для 1 узла с 1 ядром - головной модуль
	Предназначен для выполнения всех составных фаз алгоритма.

	Входные данные: таблица input.csv, содержащая в себе временной ряд
	Выходные данные: таблица output.csv, содержащая в себе индексы всех диапазонных диссонансов, 
	таблица S.csv, содержащая в себе матрицу нормализованных подпоследовательностей временного ряда из исходной таблицы.
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
	
	const char inputFileName[] = "input2.csv";
	MPI_Status stat;

	int m = 1000;
	int n = 10;
	float R = 1;
	int p = 4;

	int N;
	float dq = 1;
	


	const float r = R * R;
	//Старт работы программы

	printf("Start\n");

	int size, rank;
	int t_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//Выделение памяти для временного ряда
	float* t;
	int m_local;
	//Считывание временного ряда и передача его на узлы
	if (rank == 0)
	{
		int m_global = 0;
		FILE* file = fopen(inputFileName, "r");
		if (size != 1)
		{
			
			m_global = (m - n + 1) / (size) + (n-1);
			m_local  = (m - n + 1) % (size) + m_global;


			for (int k = 1; k < size; k++)
			{
				MPI_Send(&m_global, 1, MPI_INT, k, 0, MPI_COMM_WORLD);
			}

			t = (float*)calloc(m_local, sizeof(float));

			

			for (int k = 1; k < size; k++)
			{
				//считываем отрезок временного ряда для того, чтобы передать его на узел k
				for (int i = 0; i < m_global; i++)
				{
					if (k != 1 && i < n-1)
					{
						t[i] = t[m_global - n-1 + i];
					}
					else
					{
						fscanf(file, "%f", &t[i]);
					}
				}
				//передаём отрезок на узел k
				MPI_Send(t, m_global, MPI_FLOAT, k, 1, MPI_COMM_WORLD);
			}
			
		}
		else
		{
			m_local = m;
			t = (float*)calloc(m_local, sizeof(float));
		}
		for (int i = 0; i < m_local; i++)
		{
			if (i < n - 1 && m_global != 0)
			{
				t[i] = t[m_global - n + 1 + i];
			}
			else
			{
				fscanf(file, "%f", &t[i]);
			}
			
		}
	}
	else
	{
		MPI_Recv(&m_local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
		t = (float*)calloc(m_local, sizeof(float));
		MPI_Recv(t, m_local, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &stat);
		
	}
	//printf("---------------\n[%d]\nm_local = %d\n", rank,m_local);
	//for (int i = 0; i < m_local; i++)printf("%f ", t[i]); printf("\n");
	N = m_local - n + 1;
	int L = (int)(dq * N + 1);
	
	
	
	
	//Выделение памяти для матрицы подпоследовательностей
	float **S;
	S = (float**)calloc(N, sizeof(float*));
	for (int i = 0; i < N; i++)
	{
		S[i] = (float*)calloc(n, sizeof(float));
	}

	//Заполнение матрицы подпоследовательностей (каждая подпоследовательность нормализуется)
	fill_S(t, S, n, N);
	
	//print_matrix(S, N, n);

	//Выделение памяти для списка кандидатов и матрицы кандидатов
	
	
	
	//Фаза поиска
	

	

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
	float* C = (float*)calloc(sum*n, sizeof(float));

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







	//Выделение памяти для списка диссонансов
	int D;
	int *Discords = (int*)calloc(H, sizeof(int));

	
	int sum_H = sum;


	//Фаза уточнения
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

	//Вывод результатов на экран и в файл
	if (rank == 0)
	{
		printf("Find %d candidats\n", sum_H);
		printf("Find %d discords\n", D);
		writeDiscords("output.txt", D, Discords);
		//Окончание работы программы
		
	}


	MPI_Finalize();
	return 0;
}        