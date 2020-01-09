/*
	��������� ����� (2019 �)
	������ ������������� ��������� ������ ����������� �� ��������� ���� ��� 1 ���� � ����������� ������ - �������� ������
	������������ ��� ���������� ���� ��������� ��� ���������.

	������� ������: ������� input.csv, ���������� � ���� ��������� ���
	�������� ������: ������� output.csv, ���������� � ���� ������� ���� ����������� �����������, 
	������� S.csv, ���������� � ���� ������� ��������������� ���������������������� ���������� ���� �� �������� �������.
*/

#include <stdio.h>
#include <cstdlib> 
#include <iostream>
#include "config.h"
#include "preprocessing.h"
#include "refinement.h"
#include "selection.h"
#include "IOdata.h"
#include "ED.h"
#include <ctime>
int main()
{
	//����� ������ ���������


	//��������� ������ ��� ���������� ����
	float *t = (float*)calloc(m, sizeof(float));
	
	//���������� ���������� ����
	getTimeSeries(inputFileName, m, t);
	printf("Read t\nTime: %d\n\n", clock());
	//��������� ������ ��� ������� ����������������������
	float **S;
	S = (float**)calloc(N, sizeof(float*));
	for (int i = 0; i < N; i++)
	{
		S[i] = (float*)calloc(n, sizeof(float));
	}

	//���������� ������� ���������������������� (������ ��������������������� �������������), �������� ������� ������������ � ���� "S.csv"
	fill_S(t, S, n, N);
	write_S("S.csv", N, n, S);
	printf("Fill S\nTime: %d\n\n", clock());
	//��������� ������ ��� ������ ���������� � ������� ����������
	int H;
	int *Candidats = (int*)calloc(L,sizeof(int));
	float **C = (float**)calloc(L, sizeof(float*));
	
	//���� ������
	printf("Do selection phase\n");
	selection(S, C, r, N, n, L, p, &H, Candidats);
	printf("Find %d candidats\n", H);
	printf("Time: %d\n\n", clock());
	//��������� ������ ��� ������ �����������
	int D;
	int *Discords = (int*)calloc(H, sizeof(int));

	//���� ���������
	printf("Do refinement phase\n");
	refinement(S, C, Candidats, Discords, r, N, n, H, &D, p);
	printf("Find %d discords\n", D);
	printf("Time: %d\n\n", clock());
	//����� ����������� � ����
	writeDiscords("output.txt", D, Discords);

	//��������� ������ ���������
	printf("Finish\nTime: %d\n\n", clock());
	system("pause");
	return 0;
}