/*
	Александр Гренц (2019 г)
	Версия параллельного алгоритма поиска диссонансов во временном ряде для 1 узла с несколькими ядрами - головной модуль
	Предназначен для выполнения всех составных фаз алгоритма.

	Входные данные: таблица input.csv, содержащая в себе временной ряд
	Выходные данные: таблица output.csv, содержащая в себе индексы всех диапазонных диссонансов, 
	таблица S.csv, содержащая в себе матрицу нормализованных подпоследовательностей временного ряда из исходной таблицы.
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
	//Старт работы программы


	//Выделение памяти для временного ряда
	float *t = (float*)calloc(m, sizeof(float));
	
	//Считывание временного ряда
	getTimeSeries(inputFileName, m, t);
	printf("Read t\nTime: %d\n\n", clock());
	//Выделение памяти для матрицы подпоследовательностей
	float **S;
	S = (float**)calloc(N, sizeof(float*));
	for (int i = 0; i < N; i++)
	{
		S[i] = (float*)calloc(n, sizeof(float));
	}

	//Заполнение матрицы подпоследовательностей (каждая подпоследовательность нормализуется), итоговая матрица выписывается в файл "S.csv"
	fill_S(t, S, n, N);
	write_S("S.csv", N, n, S);
	printf("Fill S\nTime: %d\n\n", clock());
	//Выделение памяти для списка кандидатов и матрицы кандидатов
	int H;
	int *Candidats = (int*)calloc(L,sizeof(int));
	float **C = (float**)calloc(L, sizeof(float*));
	
	//Фаза поиска
	printf("Do selection phase\n");
	selection(S, C, r, N, n, L, p, &H, Candidats);
	printf("Find %d candidats\n", H);
	printf("Time: %d\n\n", clock());
	//Выделение памяти для списка диссонансов
	int D;
	int *Discords = (int*)calloc(H, sizeof(int));

	//Фаза уточнения
	printf("Do refinement phase\n");
	refinement(S, C, Candidats, Discords, r, N, n, H, &D, p);
	printf("Find %d discords\n", D);
	printf("Time: %d\n\n", clock());
	//Вывод результатов в файл
	writeDiscords("output.txt", D, Discords);

	//Окончание работы программы
	printf("Finish\nTime: %d\n\n", clock());
	system("pause");
	return 0;
}