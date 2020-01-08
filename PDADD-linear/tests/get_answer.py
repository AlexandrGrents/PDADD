import csv
numberFile = "2";
def ED(a,b):
	res = 0
	for i in range(min(len(a),len(b))):
		res += (a[i]-b[i])**2
	return res
with open ("answers/answer"+numberFile+".txt", "w", newline = '') as fout:
	with open("matrics S/S"+numberFile+".csv","r") as fin:
		R = 1
		r = R**2
		reader = csv.reader(fin, delimiter = ';')
		writer = csv.writer(fout)
		s = []
		for row in reader:
			s.append(list(map(float,row)))
		n = len(s)
		print ("n = ",n)
		# k = list(range(n))
		# k.remove(0)
		# print(k)
		dists = []
		for i in range(n):
			k = list(range(n))
			k.remove(i)
			# print (k)
			min_ED =100000
			p=-1
			for j in k:
				ed = ED(s[i],s[j])
				if min_ED>ed:
					min_ED = ed
					p = j
			dist = min_ED
			print (i,"->",p)
			dists.append(dist)
			if (dist>r):
				writer.writerow([i])
		dists.sort()
		print((dists[int(0.95*n)])**(0.5))
		