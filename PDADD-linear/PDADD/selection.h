/*
	јлександр √ренц (2019 г)
	¬ерси€ параллельного алгоритма поиска диссонансов во временном р€де дл€ 1 узла с 1 €дром - модуль фазы поиска диссонансов
	ѕредназначен дл€ нахождени€ кандидатов в диссонасы.

	‘ункции:
		findCandidats:
			ѕредназначена дл€ параллельного поиска кандидатов в диссонансы
			¬ходные данные: две подпоследовательности вещественных чисел одинаковой длины, их длина
			¬ыходные данные: квадрат евклидова рассто€ни€ между этими подпоследовательност€ми

		getCandidats
			ѕредназначена дл€ формировани€ списка кандидатов
		get_C

		selection
*/
#include <malloc.h>
#include "ED.h"
#include "config.h"
#include <stdio.h>

void findCandidats(float **S, float r, int N, int n, int L, int p, int **I, int *Insert, int *Bottom, int *Count);

void getCandidats(int **I, int *Candidats, int *Bottom, int *Count, int L, int p, int *H);

void get_C(float **S, float **C, int *Candidats, int *H, int n);

void selection(float **S, float **C, float r, int N, int n, int L, int p, int *H, int *Candidats);