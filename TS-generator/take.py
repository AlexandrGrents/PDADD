n = 100000000
i = 0
with open ("input.csv",'w') as fout:
	with open("ts20202516454.csv","r") as fin:
		for i in range(n):
			s = fin.readline()
			fout.write(s)	