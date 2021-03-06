/*
	Александр Гренц (2019 г)
	Версия параллельного алгоритма поиска диссонансов во временном ряде  PDADD для 1 узла с 1 ядром - модуль вычисления квадрата евклидова расстояния
	Предназначен для вычисления квадрата евклидова расстояния.

	Функции:
	ED_sqr - предназначена для вычисления квадрата евклидова расстояния
*/
// #include <cmath>

/*
	ED_sqr
	Предназначена для вычисления квадрата евклидова расстояния
	Входные данные: две подпоследовательности вещественных чисел одинаковой длины, их длина
	Выходные данные: квадрат евклидова расстояния между этими подпоследовательностями
*/
double ED_sqr(float *firstSubsequence, float *secondSubsequence, int n);
