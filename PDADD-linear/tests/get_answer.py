import csv
def ED(a,b):
	res = 0
	for i in range(min(len(a),len(b))):
		res += (a[i]-b[i])**2
	return res
with open ("answers/answer2.csv", "w", newline = '') as fout:
	with open("input/S2.csv","r") as fin:
		r = 0.05**2
		reader = csv.reader(fin, delimiter = ';')
		writer = csv.writer(fout)
		s = []
		for row in reader:
			s.append(list(map(float,row[:10])))
		n = len(s)

		# k = list(range(n))
		# k.remove(0)
		# print(k)
		dists = []
		for i in range(n):
			k = list(range(n))
			k.remove(i)
			# print (k)
			dist = min([ED(s[i],s[j]) for j in k])
			dists.append(dist)
			if (dist>r):
				writer.writerow([i])
			# print (dist)
		