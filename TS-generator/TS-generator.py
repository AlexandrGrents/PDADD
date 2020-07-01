# -*- coding: utf-8 -*-
import datetime
import random
from PIL import Image, ImageDraw

from argparse import ArgumentParser

# parser = ArgumentParser()

# parser.add_argument("m", type=int)
# parser.add_argument("n", type=int)
# parser.add_argument("p", type=float)

# args = parser.parse_args()

# n = args.m
# m = args.n
# probabilityAnomaly = args.p

n = 128000000
m = 128
probabilityAnomaly = 0.001

now = datetime.datetime.now()
strNow = str(now.year)+str(now.month)+str(now.day) +str(now.hour)+str(now.minute)+str(now.second)
# tsName = "ts" + strNow + ".csv"
tsName = "ECGmillion_128.csv"
marksName = "marks"

delimiter = ";"

# n = int(input("Длина ряда: "))

# m = int(input("Длина подпоследовательнсоти: "))


# probabilityAnomaly = float (input("Вероятность диссонанса: "))

print (probabilityAnomaly)
def thisIsAnomaly():
    global probabilityAnomaly
    return random.random()<probabilityAnomaly
def getSmallAlpha(m):
    return [random.uniform(0.95,1.05) for i in range(m)]
def getBigAlpha(m):
    return [random.uniform(0.1,1.9) for i in range(m)]
def randSquence():
	global left, right, m
	return [random.uniform(left,right) for i in range(m)]
left = 20
right = 120
subsequenceCount = n//m
Out = subsequenceCount//100
normalTSsubsequence  = [random.uniform(left,right) for i in range(m)]

normalTSsubsequence = [-0.066000,
-0.036000,
-0.024000,
-0.048000,
-0.012000,
0.006000,
-0.060000,
-0.042000,
0.000000,
-0.042000,
-0.042000,
-0.072000,
-0.060000,
-0.054000,
-0.054000,
-0.072000,
-0.078000,
-0.042000,
-0.048000,
-0.078000,
-0.096000,
-0.072000,
-0.030000,
-0.030000,
-0.114000,
-0.072000,
-0.072000,
-0.024000,
-0.060000,
-0.072000,
-0.048000,
-0.066000,
-0.018000,
0.042000,
0.006000,
0.006000,
0.072000,
0.066000,
0.084000,
0.096000,
0.120000,
0.132000,
0.096000,
0.048000,
0.102000,
0.042000,
-0.012000,
-0.054000,
-0.048000,
-0.084000,
-0.066000,
-0.072000,
-0.144000,
-0.204000,
0.425000,
0.545000,
0.168000,
0.132000,
0.150000,
0.156000,
0.150000,
0.030000,
0.000000,
-0.048000,
-0.078000,
-0.042000,
-0.024000,
-0.108000,
-0.096000,
-0.072000,
-0.036000,
-0.066000,
-0.084000,
-0.024000,
-0.048000,
-0.060000,
-0.024000,
-0.030000,
-0.030000,
0.024000,
0.000000,
0.006000,
0.018000,
0.048000,
0.024000,
0.072000,
0.042000,
0.108000,
0.030000,
0.018000,
0.036000,
0.090000,
0.030000,
0.006000,
0.054000,
0.060000,
-0.012000,
0.012000,
0.018000,
0.018000,
0.018000,
0.006000,
0.030000,
0.012000,
0.012000,
-0.006000,
0.000000,
0.030000,
-0.006000,
0.036000,
0.012000,
-0.018000,
-0.012000,
0.006000,
-0.006000,
-0.024000,
0.000000,
-0.018000,
-0.024000,
-0.006000,
-0.024000,
0.006000,
0.018000,
0.006000,
0.000000,
0.000000,
0.012000,
0.030000]


sizeX = n*2
sizeY = 500
dotPerX = 2
dotPerY = (sizeY)/((right+left)//2)
#image = Image.new("RGB", (sizeX, sizeY), (255, 255, 255))
#draw = ImageDraw.Draw(image)
with open (tsName,"w") as tsFile:
    print("Подготовка к работе:")
    print("|"*100)
    print("Прогресс выполнения:")
    for i in range(subsequenceCount):
        if (Out!=0):
            if (i%Out==0):
                print("|",end="")
        isAnomaly = thisIsAnomaly()
        if isAnomaly:
            alpha = getBigAlpha(m)
        else:
            alpha = getSmallAlpha(m)
        #print ("alpha: ",alpha)
        for j in range(m):
            element = normalTSsubsequence[j]*alpha[j]
            #tsFile.write(str(element)+delimiter+str(int(isAnomaly))+"\n")
            tsFile.write(str(element)+"\n")
            
            #if (i!=0 and j==0):
                #draw.line(((lastX,lastY),int(dotPerX*(i*m+j)),int(dotPerY*element)),fill=color, width = 1)
            
            lastX = int(dotPerX*(i*m+j))
            lastY = int(dotPerY*element)
    

    alpha = getSmallAlpha(m)
    for j in range(n%m):
        element = normalTSsubsequence[j]*alpha[j]
        tsFile.write(str(element)+"\n")

    print("\nEnd")

    #draw.line(((0,sizeY-2),(sizeX, sizeY-2)), fill = (0,0,0), width=2)
    #draw.line(((0,0),(0, sizeY)), fill = (0,0,0), width=2)
    

#image.save("graphic"+strNow+".bmp")
#image.show()
