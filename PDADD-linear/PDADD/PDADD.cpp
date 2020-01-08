/*
	Александр Гренц (2019 г)
	Версия параллельного алгоритма поиска диссонансов во временном ряде для 1 узла с 1 ядром - головной модуль
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

int main()
{
	//Старт работы программы


	//Выделение памяти для временного ряда
	float *t = (float*)calloc(m, sizeof(float));
	
	//Считывание временного ряда
	getTimeSeries(inputFileName, m, t);

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
	
	//Выделение памяти для списка кандидатов и матрицы кандидатов
	int H;
	int *Candidats = (int*)calloc(L,sizeof(int));
	float **C = (float**)calloc(L, sizeof(float*));
	
	//Фаза поиска
	selection(S, C, r, N, n, L, p, &H, Candidats);

	//Выделение памяти для списка диссонансов
	int D;
	int *Discords = (int*)calloc(H, sizeof(int));

	//Фаза уточнения
	refinement(S, C, Candidats, Discords, r, N, n, H, &D, p);

	//Вывод результатов на экран и в файл
	printf("Find %d candidats\n", H);
	printf("Find %d discords\n", D);
	writeDiscords("output.txt", D, Discords);

	//Окончание работы программы
	system("pause");
	return 0;
}