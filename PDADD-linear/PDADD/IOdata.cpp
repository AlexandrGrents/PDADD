#include <stdio.h>
void getTimeSeries(const char *filename, int m, float *t) 
{
	FILE *file = fopen(filename, "r");
	for (int i = 0; i < m; i++)
	{
		fscanf(file, "%f", &t[i]);
	}
	fclose(file);
}
void writeDiscords(const char *filename, int D, int *Discords)
{
	FILE *file = fopen(filename, "w");
	for (int i = 0; i < D; i++)
	{
		fprintf(file, "%d\n", Discords[i]);
	}
	fclose(file);
}
void write_S(const char *filename, int N, int n, float **S)
{
	FILE *file = fopen(filename, "w");

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fprintf(file, "%f;", S[i][j]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
}