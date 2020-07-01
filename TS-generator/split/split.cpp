#include <iostream>
#include "stdlib.h"
#include <stdio.h>
#include <cstdlib> 
using namespace std;

int main(int argc, char const *argv[])
{
	int n,m,size;
	cout << "argc = " << argc << endl;
	if (argc>1)
	{
		m = atoi(argv[2]);
		n = atoi(argv[3]);
		size = atoi(argv[4]);
	}

	

	int m_global, m_local;

	m_global = (m - n + 1) / (size) + (n-1);
	m_local  = (m - n + 1) % (size) + m_global;

	cout << "N = " << m-n+1 << endl <<
			"m_global = " << m_global << endl <<
			"m_local  = " << m_local << endl;

	FILE* file = fopen(argv[1], "r");
	FILE* out;
	char f_name[] = "input0000.csv";

	float *t = (float*)calloc(m_local, sizeof(float));


	for (int k = 1; k <= size; k++)
	{
		cout <<"["<<k<<"]\n";
		if (k !=size)
		{
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
			f_name[5] = ('0'+k/1000);
			f_name[6] = ('0'+(k/100)%10);
			f_name[7] = ('0'+(k/10)%10);
			f_name[8] = ('0'+k%10);
		}
		else
		{
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
			f_name[5] = '0';
			f_name[6] = '0';
			f_name[7] = '0';
			f_name[8] = '0';
		}
		

		out = fopen(f_name,"w");

		for (int i =0; i<m_global; i++)
		{
			fprintf(out, "%f\n", t[i]);
			// printf("%f\n", t[i]);
			
		}
		cout << "--------------------------\n";
		fclose(out);
	}
	
	fclose(file);
	return 0;
}