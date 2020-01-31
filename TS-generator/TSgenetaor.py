# -*- coding: utf-8 -*-
import datetime
import random
from PIL import Image, ImageDraw
now = datetime.datetime.now()
strNow = str(now.year)+str(now.month)+str(now.day) +str(now.hour)+str(now.minute)+str(now.second)
tsName = "ts" + strNow + ".csv"
marksName = "marks"

delimiter = ";"
n = int(input("Длина ряда: "))

m = int(input("Длина подпоследовательнсоти: "))


probabilityAnomaly = float (input("Вероятность диссонанса: "))

print (probabilityAnomaly)
def thisIsAnomaly():
    global probabilityAnomaly
    return random.random()<probabilityAnomaly
def getSmallAlpha(m):
    return [random.uniform(0.95,1.05) for i in range(m)]
def getBigAlpha(m):
    return [random.uniform(0.1,1.9) for i in range(m)]

left = 20
right = 120
subsequenceCount = n//m
Out = subsequenceCount//100
normalTSsubsequence  = [random.uniform(left,right) for i in range(m)]


sizeX = n*2
sizeY = 500
dotPerX = 2
dotPerY = (sizeY)/((right+left)//2)
image = Image.new("RGB", (sizeX, sizeY), (255, 255, 255))
draw = ImageDraw.Draw(image)
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
            tsFile.write(str(element)+delimiter+str(int(isAnomaly))+"\n")
            if (isAnomaly):
                color = (0,0,255)
            else:
                color = (255,0,0)
            if (i!=0 and j==0):
                draw.line(((lastX,lastY),int(dotPerX*(i*m+j)),int(dotPerY*element)),fill=color, width = 1)
            
            lastX = int(dotPerX*(i*m+j))
            lastY = int(dotPerY*element)
        
    print()
    #draw.line(((0,sizeY-2),(sizeX, sizeY-2)), fill = (0,0,0), width=2)
    #draw.line(((0,0),(0, sizeY)), fill = (0,0,0), width=2)
    

image.save("graphic"+strNow+".bmp")
image.show()
