/*
	��������� ����� (2019 �)
	������ ������������� ��������� ������ ����������� �� ��������� ����  PDADD ��� 1 ���� � 1 ����� - ������ ���������� �������� ��������� ����������
	������������ ��� ���������� �������� ��������� ����������.

	�������:
	ED_sqr - ������������� ��� ���������� �������� ��������� ����������
*/
// #include <cmath>

/*
	ED_sqr
	������������� ��� ���������� �������� ��������� ����������
	������� ������: ��� ��������������������� ������������ ����� ���������� �����, �� �����
	�������� ������: ������� ��������� ���������� ����� ����� �����������������������
*/
double ED_sqr(float *firstSubsequence, float *secondSubsequence, int n);
double EarlyED_sqr(float *firstSubsequence, float *secondSubsequence, int n, float r);
double ED_norm(float *first, float *second, int n, double u1, double u2, double q1, double q2);