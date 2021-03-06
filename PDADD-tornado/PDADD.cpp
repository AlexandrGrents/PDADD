#include "mpi.h"
#include <stdio.h>
#include <cstdlib> 
#include <iostream>
#include <string.h>
#include "preprocessing.h"
#include "refinement.h"
#include "selection.h"
#include "IOdata.h"
#include "ED.h"
#include "debug.h"
#include "system.h"

int main(int argc, char** argv)
{
    MPI_Status stat;
    char* inputFileName = argv[1];
    const int m = atoi(argv[2]);
    const int n = atoi(argv[3]);
    float R = atof(argv[4]);
    const int p = atoi(argv[5]);

    
    
	int N;
	float dq = 1;

	const float r = R * R;

	printf("Start\n");

	int size, rank;
	int t_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
	float* t;
	int m_local;

    if (rank == 0)
    {
        m_local = (m - n + 1) / (size)+(n - 1) + (m - n + 1) % (size);
    	printf("m = %d,    n = %d,      R = %f,      p = %d\n", m,n,R,p);
    	printf("Rank 0, start, time: %lf\n",MPI_Wtime());
    }
    else
    {
        m_local = (m - n + 1) / (size)+(n - 1);
    }
    
    
    int len_name_file = strlen(inputFileName);
    inputFileName[len_name_file - 8] = (char)('0' + rank / 1000);
    inputFileName[len_name_file - 7] = (char)('0' + (rank / 100) % 10);
    inputFileName[len_name_file - 6] = (char)('0' + (rank / 10) % 10);
    inputFileName[len_name_file - 5] = (char)('0' + rank % 10);
    
    printf(inputFileName);
    
    FILE* file = fopen(inputFileName, "r");
    
    printf("%d\n", m_local);
    
    t = (float*)_align_malloc(m_local * sizeof(float));
    
    for (int i = 0; i < m_local; i++)
    {
        fscanf(file, "%f", &t[i]);
    }
    
    fclose(file);
    if (rank == 0) printf("Rank 0, end read, time: %lf\n",MPI_Wtime());
    
    

	N = m_local - n + 1;
	int L = (int)(dq * N + 1);
	
	

	if (rank == 0) printf("Rank 0, start fill u and q, time: %lf\n",MPI_Wtime());
	
	
	float **S = (float**)_align_malloc(N * sizeof(float*));
	for (int i = 0; i < N; i++)
	{
		S[i] = (float*)_align_malloc(n * sizeof(float));
	}

	
	fill_S(t, S, n, N);
	if (rank == 0) printf("Rank 0, end fill, time: %lf\n",MPI_Wtime());
    if (rank == 0) 
    {
    	for (int i =0; i<n; i++)
    	{
    		printf("%f ",S[0][i] );
    	}
    	printf("\n");
    	for (int i =0; i<n; i++)
    	{
    		printf("%f ",S[100][i] );
    	}
    	printf("\n");
    
    }

	int add = 0;
	if (rank == 0)
	{
		add = m - n + 1 - N;
	}
	else
	{
		add = N * (rank - 1);
	}

	int* Insert = (int*)_align_malloc(p * sizeof(int));
    int* Bottom = (int*)_align_malloc(p * sizeof(int));
    int* Count = (int*)_align_malloc(p * sizeof(int));
	for (int i = 0; i < p; i++)
    {
        Insert[i] = 0;
        Bottom[i] = 1;
        Count[i] = 0;
    }

	int** I = (int**)_align_malloc(p * sizeof(int*));
	for (int i = 0; i < p; i++)
    {
        I[i] = (int*)_align_malloc(L * sizeof(int));
        for (int j = 0; j < L; j++)
        {
            I[i][j] = NIL;
        }
    }

    int* sizes = (int*)_align_malloc(size * sizeof(int));
   	if (rank == 0) printf("Rank 0, start 'findCandidats', time: %lf\n",MPI_Wtime());

    
    
    double time_ph1, time_ph2, tmp_time, start_time, start_time1;
    
    start_time = MPI_Wtime();
//локальная фаза поиска
	findCandidats(S, r, N, n, L, p, I, Insert, Bottom, Count);
	

	if (rank == 0) printf("Rank 0, end 'findCandidats', time: %lf\n",MPI_Wtime());

	time_ph1 = MPI_Wtime()-start_time;
	if (rank!=0)
	{
		MPI_Send(&time_ph1,1,MPI_DOUBLE,0,99,MPI_COMM_WORLD);
	}
	else
	{
		for (int i = 1; i<size; i++)
		{
			MPI_Recv(&tmp_time,1,MPI_DOUBLE,i,99,MPI_COMM_WORLD,&stat);
			if (tmp_time>time_ph1) time_ph1 = tmp_time;
		}
		printf("Time for phase 1: %.2lf\n",time_ph1);
	}

	int H = 0;

	for (int k = 0; k < p; k++)
	{
		H += Count[k];
	}
	printf("BEFORE rank = %d C = %d\n", rank, H);
	
    //выделение памяти для вспомогательных структур данных

	int* Candidats_l = (int*)_align_malloc(H * sizeof(int));
    float* C_l = (float*)_align_malloc(H * n * sizeof(float));
    bool **B_l = (bool**)_align_malloc(p * sizeof(bool*));
    int *Discords_l = (int*)_align_malloc(H * sizeof(int));
    int* other_discords_l = (int*)_align_malloc(H * sizeof(int));
    int D_l;
    for (int i = 0; i < p; i++)
    {
        B_l[i] = (bool*)_align_malloc(H * sizeof(bool));
        for (int j = 0; j < H; j++)
        {
            B_l[i][j] = true;
        }
    }
    int i=0;
    for (int k = 0; k < p; k++)
	{
		for (int j = 0; j < Bottom[k]; j++)
		{
			if (I[k][j] != NIL)
			{
				Candidats_l[i] = I[k][j];
				i++;
			}
		}
	}
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C_l[i*n+j]= S[Candidats_l[i]][j];
		}
	}
	
	//локальная фаза отсеивания

	start_time1 = MPI_Wtime();
	refinement(S, B_l, C_l, Candidats_l, Discords_l, r, N, n, H, &D_l, p, 0);

	printf("REF 1!! rank = %d time = %lf\n", rank, MPI_Wtime()-start_time1);
	H = D_l;

	
    
    printf("AFTER rank = %d C = %d\n", rank, H);
    
	sizes[rank] = H;
	for (int i = 0; i < size; i++)
	{
		if (i == rank)
		{
			for (int j = 0; j < size; j++)
			{
				if (j != rank)
                    MPI_Send(&H, 1, MPI_INT, j, 2, MPI_COMM_WORLD);
			}
		}
		else
		{
			MPI_Recv(sizes + i, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &stat);
		}
	}
	if (rank == 0) printf("Rank 0, end H-exchange, time: %lf\n",MPI_Wtime());
	int sum = 0, pre = 0;
	for (int i = 0; i < size; i++)
	{
		sum += sizes[i];
		if (i < rank)
            pre += sizes[i];
	}

	if (rank==0) printf("summar H = %d\n",sum);
	
	// выделение памяти для структур данных, нужных для обмена между узлами
	double time_for_calloc_start, time_for_calloc;
	time_for_calloc_start = MPI_Wtime();


	int* Candidats = (int*)_align_malloc(sum * sizeof(int));
    float* C = (float*)_align_malloc(sum * n * sizeof(float));
    int *Discords = (int*)_align_malloc(sum * sizeof(int));

    bool *checkDiscords = (bool*)_align_malloc(sum * sizeof(bool));
    for (int i = 0; i < sum; i++)
    {
        checkDiscords[i] = false;
    }
    int* other_discords = (int*)_align_malloc(sum * sizeof(int));
    
    bool **B = (bool**)_align_malloc(p * sizeof(bool*));
    for (int i = 0; i < p; i++)
    {
        B[i] = (bool*)_align_malloc(sum * sizeof(bool));
        for (int j = 0; j < sum; j++)
        {
            B[i][j] = true;
        }
    }

    float * u_and_q_c = (float*)_align_malloc(sum*2*sizeof(float));

    time_for_calloc = MPI_Wtime() - time_for_calloc_start;

	

    if (rank == 0) printf("Rank 0, end callocs, time: %lf\n",MPI_Wtime());

   printf("rank = %d sum = %d pre = %d add = %d\n", rank, sum, pre, add);
    
	i = pre;
	for (int j = 0; j<H; j++)
	{
		Candidats[i] = Discords_l[j];
		
		i++;
	}
	


	for (int i = pre; i < pre+H; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C[i*n+j]= S[Candidats[i]][j];
		}
		Candidats[i] += add;
	}
    
	if (rank == 0) printf("Rank 0, start Candidats-exchange, time: %lf\n",MPI_Wtime());
	int pre_now = 0;
	
//обмен локальными диссонансами между узлами
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
			MPI_Recv(Candidats + pre_now, sizes[i], MPI_INT, i, 3, MPI_COMM_WORLD, &stat);
			MPI_Recv(C + (pre_now * n), sizes[i] * n, MPI_FLOAT, i, 4, MPI_COMM_WORLD, &stat);
			
		}
		pre_now += sizes[i];
	}
	if (rank == 0) printf("Rank 0, end Candidats-exchange, time: %lf\n",MPI_Wtime());

	
	int D;
	
	int sum_H = sum;

	start_time1 = MPI_Wtime();
	if (rank == 0) printf("Rank 0, start 'refinement', time: %lf\n",MPI_Wtime());
	

	refinement(S, B, C, Candidats, Discords, r, N, n, sum_H, &D, p, add);



	if (rank == 0) printf("Rank 0, end 'refinement', time: %lf\n",MPI_Wtime());
	time_ph2 = MPI_Wtime()-start_time1;
	printf("rank = %d; time_ph_2 = %lf\n", rank, time_ph2);
	if (rank!=0)
	{
		MPI_Send(&time_ph2,1,MPI_DOUBLE,0,199,MPI_COMM_WORLD);
	}
	else
	{
		for (int i = 1; i<size; i++)
		{
			MPI_Recv(&tmp_time,1,MPI_DOUBLE,i,199,MPI_COMM_WORLD,&stat);
			if (tmp_time>time_ph2) time_ph2 = tmp_time;
		}
		printf("Time for phase 2: %.2lf\n",time_ph2);
	}
    
    
	sizes[rank] = D;
    if (rank == 0) printf("Rank 0, end D-exchange, time: %lf\n",MPI_Wtime());
	for (int i = 0; i < size; i++)
	{
		if (i == rank)
		{
			for (int j = 0; j < size; j++)
			{
				if (j != rank)
                    MPI_Send(&D, 1, MPI_INT, j, 2, MPI_COMM_WORLD);
			}
		}
		else
		{
			MPI_Recv(sizes + i, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &stat);
		}
	}
	if (rank == 0) printf("Rank 0, end D-exchange, time: %lf\n",MPI_Wtime());
	sum = 0;
	pre = 0;
	for (int i = 0; i < size; i++)
	{
		sum += sizes[i];
		if (i < rank)
            pre += sizes[i];
	}

    
	int max = 0, tmp;
	bool fl;
    
	printf("rank = %d, D = %d\n",rank, D);
    
	if (rank == 0) printf("Rank 0, start Discords-exchange, time: %lf\n",MPI_Wtime());
	if (rank != 0)
        MPI_Send(Discords, D, MPI_INT, 0, 7, MPI_COMM_WORLD);
	else
	{
		for (int k = 1; k < size; k++)
		{
			MPI_Recv(other_discords, sizes[k], MPI_INT, k, 7, MPI_COMM_WORLD, &stat);
			i = 0;
			while (i < D)
			{
				fl = true;
				for (int j = 0; j < sizes[k]; j++)
				{
					if (Discords[i] == other_discords[j])
					{
						fl = false;
                        checkDiscords[i] = true;
						break;
					}
				}
				if (fl)
				{
					
					Discords[i] = Discords[D - 1];
                    checkDiscords[i] = checkDiscords[D - 1];
					D--;
					i--;
				}
				i++;
			}
		}
        
      
        
        if (size > 1) 
        {
            int i = 0;
            while (i < D)
            {
                if (!checkDiscords[i])
                {
                    Discords[i] = Discords[D - 1];
                    D--;
                    i--;
                }
                i++;
            }
        }
        
	}
	if (rank == 0) printf("Rank 0, end Discords-exchange, time: %lf\n",MPI_Wtime());
	if (rank == 0)
	{
        printf("Total Runtime of finding discords = %.2lf\n", MPI_Wtime() - start_time - time_for_calloc);
		printf("Find %d candidats\n", sum_H);
		printf("Find %d discords\n", D);
		writeDiscords("output.txt", D, Discords);
	}

	MPI_Finalize();
	return 0;
}        
