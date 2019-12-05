import csv
def ED(a,b):
	res = 0
	for i in range(min(len(a),len(b))):
		res += (a[i]-b[i])**2
	return res
with open("S.csv","r") as fin:
	reader = csv.reader(fin, delimiter = ';')
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
		# print (dist)
	with open("output.csv","r") as answ:
		reader2 = csv.reader(answ)
		for ans in reader2:
			if (ans != ''):
				ans = int(ans[0])
				print(ans,'---->\t',round(dists[ans],5), '\t{',dists[ans]>(0.05)**2,'}')
	print(935,'---->\t',round(dists[935],5), '\t{',dists[935]>(0.05)**2,'}')
	print("Max: ",max(dists))